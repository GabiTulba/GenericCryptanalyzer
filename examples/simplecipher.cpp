#include "src/box/sbox.h"
#include "src/box/xorbox.h"
#include "src/cipheranalyzer.h"
#include "src/helpers/helpers.h"
#include "src/roundfunction.h"
#include <boost/dynamic_bitset.hpp>
#include <builders/abstractboxbuilder.h>
#include <builders/cipheranalyzerbuilder.h>
#include <builders/identityboxbuilder.h>
#include <builders/pboxbuilder.h>
#include <builders/roundbuilder.h>
#include <builders/sboxbuilder.h>
#include <cstdio>

using namespace std;
using namespace boost;

const int round_cnt = 3;
const int max_weight = 16;
const int max_queue_size = 1 << 20;
const int threads = 8;
const int max_inputs = 4;

CipherAnalyzerBuilderPtr create_cipher_builder() {
    vector<size_t> sbox{0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7};
    vector<size_t> pbox{0x0, 0x4, 0x8, 0xc, 0x1, 0x5, 0x9, 0xd, 0x2, 0x6, 0xa, 0xe, 0x3, 0x7, 0xb, 0xf};

    AbstractBoxBuilderPtr pbox_builder = make_pbox_builder(pbox);
    AbstractBoxBuilderPtr sbox_builder = make_sbox_builder(sbox, true);
    AbstractBoxBuilderPtr identitybox_builder = make_identitybox_builder(16);

    RoundBuilderPtr round_builder = make_round_builder();
    round_builder->add_builder("Src", identitybox_builder);
    round_builder->add_builder("Sbox1", sbox_builder);
    round_builder->add_builder("Sbox2", sbox_builder);
    round_builder->add_builder("Sbox3", sbox_builder);
    round_builder->add_builder("Sbox4", sbox_builder);
    round_builder->add_builder("Pbox", pbox_builder);

    round_builder->add_connection("Src", "Sbox1", BitsRange(0, 4), BitsRange(0, 4));
    round_builder->add_connection("Src", "Sbox2", BitsRange(4, 4), BitsRange(0, 4));
    round_builder->add_connection("Src", "Sbox3", BitsRange(8, 4), BitsRange(0, 4));
    round_builder->add_connection("Src", "Sbox4", BitsRange(12, 4), BitsRange(0, 4));
    round_builder->add_connection("Sbox1", "Pbox", BitsRange(0, 4), BitsRange(0, 4));
    round_builder->add_connection("Sbox2", "Pbox", BitsRange(0, 4), BitsRange(4, 4));
    round_builder->add_connection("Sbox3", "Pbox", BitsRange(0, 4), BitsRange(8, 4));
    round_builder->add_connection("Sbox4", "Pbox", BitsRange(0, 4), BitsRange(12, 4));
    round_builder->set_src("Src");
    round_builder->set_dst("Pbox");

    map<string, RoundBuilderPtr> round_constrs{{"Round", round_builder}};

    vector<string> round_names;

    for (int i = 0; i < round_cnt; i++) {
        round_names.push_back("Round");
    }

    CipherAnalyzerBuilderPtr cipher_builder = make_cipher_analyzer_builder();
    cipher_builder->add_round_builder("Round", round_builder);
    for (int i = 0; i < round_cnt; i++) {
        cipher_builder->add_round_name("Round");
    }
    return cipher_builder;
}

void job(CipherAnalyzerBuilderPtr &cipher_builder, CipherAnalyzerPtr &cipher, pthread_mutex_t *cipher_lock,
         dynamic_bitset<> &input, int thread_idx, sem_t *queue_semaphore) {
    pthread_mutex_lock(cipher_lock);

    cipher->set_input(input, BitsRange(0, cipher->get_input_size()));
    auto [output, prob] = cipher->get_next_state();

    while (output.size() > 0) {
        printf("[%02d] in: 0x%s (%02ld),\tout: 0x%s (%02ld) -> prob %.30lf\n", thread_idx,
               convert_to_hex_string(input).c_str(), input.count(), convert_to_hex_string(output).c_str(),
               output.count(), prob);

        cipher_builder->update_global_thresh(prob);

        auto [next_output, next_prob] = cipher->get_next_state();
        output = next_output, prob = next_prob;
    }

    sem_post(queue_semaphore);

    pthread_mutex_unlock(cipher_lock);
}

int main() {
    int thread_idx = 0;
    sem_t queue_semaphore;
    asio::thread_pool worker_pool(threads);
    CipherAnalyzerBuilderPtr cipher_builder = create_cipher_builder();
    vector<CipherAnalyzerPtr> ciphers;
    vector<pthread_mutex_t> cipher_locks(threads);
    vector<dynamic_bitset<>> initial_inputs;

    sem_init(&queue_semaphore, 0, max_queue_size);

    for (int i = 0; i < threads; i++) {
        ciphers.push_back(cipher_builder->build());
        pthread_mutex_init(&cipher_locks[i], NULL);
        if (i == 0) {
            initial_inputs = ciphers[0]->calculate_opt_probs(max_weight, max_inputs, threads, max_queue_size);
            cipher_builder->set_opt_probs(ciphers[0]->get_opt_probs());
        }
    }

    for (auto &initial_input : initial_inputs) {
        sem_wait(&queue_semaphore);
        asio::post(worker_pool, bind(job, cipher_builder, ciphers[thread_idx], &cipher_locks[thread_idx], initial_input,
                                     thread_idx, &queue_semaphore));
        thread_idx++;
        if (thread_idx == threads) {
            thread_idx = 0;
        }
    }

    for (unsigned i = 1; i <= max_weight; i++) {
        dynamic_bitset<> input = dynamic_bitset<>(ciphers[0]->get_input_size());
        input.set(0, i, 1);
        printf("Trying with %d bits\n", i);

        do {
            sem_wait(&queue_semaphore);

            asio::post(worker_pool, bind(job, cipher_builder, ciphers[thread_idx], &cipher_locks[thread_idx], input,
                                         thread_idx, &queue_semaphore));
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

    return 0;
}
