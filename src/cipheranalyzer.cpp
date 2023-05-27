#include "cipheranalyzer.h"

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

CipherAnalyzer::CipherAnalyzer(vector<RoundFunctionPtr> rounds, size_t input_max_hamming_weight, double global_thresh,
                               vector<double> opt_probs)
    : global_thresh(global_thresh), opt_probs(opt_probs), rounds(rounds) {
    curr_idx = 0;
    round_probs = vector<double>(rounds.size(), 1.0);

    if (opt_probs.size() < rounds.size()) {
        vector<RoundFunctionPtr> rounds_pref(rounds.begin(), rounds.begin() + rounds.size() - 1);

        CipherAnalyzerPtr prev_cipher =
            make_shared<CipherAnalyzer>(rounds_pref, input_max_hamming_weight, global_thresh, opt_probs);

        dynamic_bitset<> input(rounds[0]->src->input_size());

        for (size_t weight = 1; weight <= min(input_max_hamming_weight, rounds[0]->src->input_size()); weight++) {
            input.set(0, input.size(), 0);
            input.set(0, weight, 1);
            bool done = false;
            while (!done) {
                prev_cipher->set_input(input, {0, input.size()});
                auto entry = prev_cipher->get_next_entry();
                while (entry.first.size() > 0) {
                    entry = prev_cipher->get_next_entry();
                }
                done = !increment_bits(input);
            }
        }

        this->opt_probs = prev_cipher->opt_probs;
        this->opt_probs.push_back(prev_cipher->global_thresh);
    }
}

CipherAnalyzer::CipherAnalyzer(vector<RoundFunctionPtr> rounds, size_t input_max_hamming_weight, double global_thresh)
    : CipherAnalyzer(rounds, input_max_hamming_weight, global_thresh, vector<double>{1.0}) {
    int x = 0;
    x++;
}

ProbEntry CipherAnalyzer::get_next_state() {
    while (curr_idx < rounds.size()) {
        if (!advance_state()) {
            return {dynamic_bitset<>(0), 0.0};
        }
    }
    curr_idx--;
    global_thresh = max(global_thresh, round_probs.back());
    return {rounds.back()->dst->get_output(), round_probs.back()};
}

void CipherAnalyzer::set_input(const dynamic_bitset<> &bits, BitsRange rng) {
    rounds[0]->set_input(bits, rng);
    rounds[0]->set_threshold(global_thresh / opt_probs[rounds.size() - 1]);
    curr_idx = 0;
}
