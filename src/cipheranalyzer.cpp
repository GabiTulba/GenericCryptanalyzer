#include "cipheranalyzer.h"
#include <builders/cipheranalyzerbuilder.h>

bool CipherAnalyzer::advance_state() {
    while (curr_idx > 0 && rounds[curr_idx]->is_determined()) {
        curr_idx--;
    }

    if (curr_idx == 0 && rounds[curr_idx]->is_determined()) {
        return false;
    }

    auto round_entry = rounds[curr_idx]->get_next_state();
    auto round_out = round_entry.first;
    if (curr_idx == 0) {
        round_probs[0] = round_entry.second;
    } else {
        round_probs[curr_idx] = round_entry.second * round_probs[curr_idx - 1];
    }

    if (curr_idx > 0 && round_probs[curr_idx] * opt_probs[rounds.size() - 1 - curr_idx] < global_thresh) {
        curr_idx--;
        return true;
    } else if (curr_idx == 0 && round_probs[curr_idx] * opt_probs[rounds.size() - 1 - curr_idx] < global_thresh) {
        return false;
    }

    if (curr_idx < rounds.size() - 1) {
        rounds[curr_idx + 1]->set_input(round_out, {0, round_out.size()});
        rounds[curr_idx + 1]->set_threshold(global_thresh / round_probs[curr_idx]);
    }

    if (curr_idx < rounds.size()) {
        curr_idx++;
    }

    return true;
}

void CipherAnalyzer::update_global_thresh(double global_thresh) {
    pthread_mutex_lock(&global_thresh_lock);
    this->global_thresh = max(this->global_thresh, global_thresh);
    pthread_mutex_unlock(&global_thresh_lock);
}

CipherAnalyzer::CipherAnalyzer(map<string, RoundBuilderPtr> round_builders, vector<string> round_names,
                               double global_thresh, vector<double> opt_probs)
    : round_builders(round_builders), round_names(round_names), global_thresh(global_thresh) {
    pthread_mutex_init(&global_thresh_lock, NULL);

    this->opt_probs = opt_probs;

    for (auto &round_name : round_names) {
        rounds.push_back(round_builders[round_name]->build());
    }
    curr_idx = 0;
    round_probs = vector<double>(rounds.size(), 1.0);
}

CipherAnalyzer::CipherAnalyzer(map<string, RoundBuilderPtr> round_builders, vector<string> round_names,
                               double global_thresh)
    : CipherAnalyzer(round_builders, round_names, global_thresh, vector<double>{1.0}) {
    pthread_mutex_init(&global_thresh_lock, NULL);
}

CipherAnalyzer::~CipherAnalyzer() { pthread_mutex_destroy(&global_thresh_lock); }

void worker_job(CipherAnalyzerBuilderPtr &cipher_builder, CipherAnalyzerPtr &cipher, pthread_mutex_t *cipher_lock,
                dynamic_bitset<> input, int thread_idx, sem_t *queue_semaphore, vector<dynamic_bitset<>> *best_inputs,
                pthread_mutex_t *best_inputs_lock, size_t max_inputs) {
    pthread_mutex_lock(cipher_lock);

    cipher->set_input(input, BitsRange(0, cipher->get_input_size()));
    auto [output, prob] = cipher->get_next_state();

    while (output.size() > 0) {
        cipher_builder->update_global_thresh(prob);

        if (prob > 0) {
            printf("[%02d] in: 0x%s (%02ld),\tout: 0x%s (%02ld) -> prob %.16lf\n", thread_idx,
                   convert_to_hex_string(input).c_str(), input.count(), convert_to_hex_string(output).c_str(),
                   output.count(), prob);
        }

        pthread_mutex_lock(best_inputs_lock);

        best_inputs->push_back(input);
        if (best_inputs->size() > max_inputs) {
            best_inputs->erase(best_inputs->begin());
        }

        pthread_mutex_unlock(best_inputs_lock);

        auto [next_output, next_prob] = cipher->get_next_state();
        output = next_output, prob = next_prob;
    }

    sem_post(queue_semaphore);

    pthread_mutex_unlock(cipher_lock);
}

vector<dynamic_bitset<>> CipherAnalyzer::calculate_opt_probs(size_t input_max_hamming_weight, size_t max_inputs,
                                                             size_t threads, size_t max_queue_size) {
    if (opt_probs.size() < rounds.size()) {
        // Claculate for opt_probs for previous rounds
        CipherAnalyzerBuilderPtr cipher_builder = make_cipher_analyzer_builder();

        cipher_builder->set_opt_probs(opt_probs);
        cipher_builder->set_global_thresh(0.0);

        for (auto &[builder_name, builder] : round_builders) {
            cipher_builder->add_round_builder(builder_name, builder);
        }
        for (size_t i = 0; i < round_names.size() - 1; i++) {
            cipher_builder->add_round_name(round_names[i]);
        }

        CipherAnalyzerPtr first_cipher = cipher_builder->build();

        vector<dynamic_bitset<>> prev_best_inputs =
            first_cipher->calculate_opt_probs(input_max_hamming_weight, max_inputs, threads, max_queue_size);

        cipher_builder->set_opt_probs(first_cipher->opt_probs);

        // Calculate opt_probs for this number of rounds
        vector<dynamic_bitset<>> best_inputs;
        pthread_mutex_t best_inputs_lock;
        sem_t queue_semaphore;
        asio::thread_pool worker_pool(threads);
        vector<CipherAnalyzerPtr> ciphers;
        vector<pthread_mutex_t> cipher_locks(threads);
        size_t thread_idx = 0;

        sem_init(&queue_semaphore, 0, max_queue_size);
        pthread_mutex_init(&best_inputs_lock, NULL);

        for (int i = 0; i < threads; i++) {
            if (i == 0) {
                ciphers.push_back(first_cipher);
            } else {
                ciphers.push_back(cipher_builder->build());
            }
            pthread_mutex_init(&cipher_locks[i], NULL);
        }

        printf("Calculating best probability for %lu rounds\n", rounds.size() - 1);

        int step = 0;
        for (auto &initial_input : prev_best_inputs) {
            sem_wait(&queue_semaphore);
            asio::post(worker_pool,
                       bind(worker_job, cipher_builder, ciphers[thread_idx], &cipher_locks[thread_idx], initial_input,
                            thread_idx, &queue_semaphore, &best_inputs, &best_inputs_lock, max_inputs));
            thread_idx++;
            if (thread_idx == threads) {
                thread_idx = 0;
            }
        }

        for (size_t weight = 1; weight <= min(input_max_hamming_weight, get_input_size()); weight++) {
            printf("Trying with %lu bits\n", weight);
            dynamic_bitset<> input(get_input_size());
            input.set(0, weight, 1);

            do {
                sem_wait(&queue_semaphore);

                asio::post(worker_pool,
                           bind(worker_job, cipher_builder, ciphers[thread_idx], &cipher_locks[thread_idx], input,
                                thread_idx, &queue_semaphore, &best_inputs, &best_inputs_lock, max_inputs));
                thread_idx++;
                if (thread_idx == threads) {
                    thread_idx = 0;
                }

            } while (increment_bits(input));
        }

        worker_pool.wait();
        worker_pool.stop();
        sem_destroy(&queue_semaphore);
        for (int i = 0; i < threads; i++) {
            pthread_mutex_destroy(&cipher_locks[i]);
        }
        printf("Found best probability for %lu rounds: %.14f\n", rounds.size() - 1, first_cipher->global_thresh);

        this->opt_probs = first_cipher->opt_probs;
        this->opt_probs.push_back(first_cipher->global_thresh);

        reverse(best_inputs.begin(), best_inputs.end());
        return best_inputs;
    }
    return vector<dynamic_bitset<>>();
}

ProbEntry CipherAnalyzer::get_next_state() {
    while (curr_idx < rounds.size()) {
        if (!advance_state()) {
            return {dynamic_bitset<>(0), 0.0};
        }
    }
    curr_idx--;
    pthread_mutex_lock(&global_thresh_lock);
    global_thresh = max(global_thresh, round_probs.back());
    pthread_mutex_unlock(&global_thresh_lock);

    return {rounds.back()->dst->get_output(), round_probs.back()};
}

void CipherAnalyzer::set_input(const dynamic_bitset<> &bits, BitsRange rng) {
    rounds[0]->set_input(bits, rng);
    rounds[0]->set_threshold(global_thresh / opt_probs[rounds.size() - 1]);
    curr_idx = 0;
}

size_t CipherAnalyzer::get_input_size() { return rounds[0]->src->input_size(); }

vector<double> CipherAnalyzer::get_opt_probs() { return opt_probs; }

CipherAnalyzerPtr make_cipher_analyzer(map<string, RoundBuilderPtr> round_builders, vector<string> round_names,
                                       double global_threshold) {
    return std::make_shared<CipherAnalyzer>(round_builders, round_names, global_threshold);
}

CipherAnalyzerPtr make_cipher_analyzer(map<string, RoundBuilderPtr> round_builders, vector<string> round_names,
                                       double global_threshold, vector<double> opt_probs) {
    return std::make_shared<CipherAnalyzer>(round_builders, round_names, global_threshold, opt_probs);
}
