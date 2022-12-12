#include "cipheranalyzer.h"

bool CipherAnalyzer::advance_state() {
    while (curr_round_idx > 0 && rounds[curr_round_idx]->is_determined()) {
        curr_round_idx--;
    }

    if (curr_round_idx == 0 && rounds[curr_round_idx]->is_determined()) {
        return false;
    }

    auto round_differential = rounds[curr_round_idx]->get_next_differential();
    auto round_output = round_differential.first;
    if (curr_round_idx == 0) {
        round_probabilities[0] = round_differential.second;
    } else {
        round_probabilities[curr_round_idx] =
            round_differential.second * round_probabilities[curr_round_idx - 1];
    }

    if (curr_round_idx > 0 &&
        round_probabilities[curr_round_idx] *
                optimal_probabilities[rounds.size() - 1 - curr_round_idx] <
            global_threshold) {
        curr_round_idx--;
        return true;
    } else if (curr_round_idx == 0 &&
               round_probabilities[curr_round_idx] *
                       optimal_probabilities[rounds.size() - 1 -
                                             curr_round_idx] <
                   global_threshold) {
        return false;
    }

    if (curr_round_idx < rounds.size() - 1) {
        rounds[curr_round_idx + 1]->set_input(round_output,
                                              {0, round_output.size()});
        rounds[curr_round_idx + 1]->set_threshold(
            global_threshold /
            (round_probabilities[curr_round_idx] *
             optimal_probabilities[rounds.size() - 1 - curr_round_idx]));
    }

    if (curr_round_idx < rounds.size()) {
        curr_round_idx++;
    }

    return true;
}

bool CipherAnalyzer::increment_bits(dynamic_bitset<> &input) {
    size_t idx = 0;
    while (input[idx] == 1) {
        input[idx++] = 0;
    }
    if (idx == input.size()) {
        return false;
    }
    input[idx] = 1;
    return true;
}

CipherAnalyzer::CipherAnalyzer(
    size_t round_count, double global_threshold, string src_id, string dst_id,
    vector<double> optimal_probabilities,
    map<string, function<std::shared_ptr<AbstractBox>()>> constructors,
    map<string, vector<pair<string, pair<bits_range, bits_range>>>> connections)
    : global_threshold(global_threshold),
      optimal_probabilities(optimal_probabilities) {
    rounds = vector<std::shared_ptr<RoundFunction>>(
        round_count, std::make_shared<RoundFunction>(
                         src_id, dst_id, constructors, connections));
    curr_round_idx = 0;
    round_probabilities = vector<double>(round_count, 1.0);

    if (optimal_probabilities.size() < rounds.size()) {
        std::shared_ptr<CipherAnalyzer> previous_cipher =
            std::make_shared<CipherAnalyzer>(
                round_count - 1, global_threshold, src_id, dst_id,
                optimal_probabilities, constructors, connections);
        dynamic_bitset<> input(rounds[0]->src->input_size());
        while (increment_bits(input)) {
            previous_cipher->set_input(input, {0, input.size()});
            auto diff = previous_cipher->get_next_differential();
            while (diff.first.size() > 0) {
                diff = previous_cipher->get_next_differential();
            }
        }

        this->optimal_probabilities = previous_cipher->optimal_probabilities;
        this->optimal_probabilities.push_back(
            previous_cipher->global_threshold);
    }
}

CipherAnalyzer::CipherAnalyzer(
    size_t round_count, double global_threshold, string src_id, string dst_id,
    map<string, function<std::shared_ptr<AbstractBox>()>> constructors,
    map<string, vector<pair<string, pair<bits_range, bits_range>>>> connections)
    : CipherAnalyzer(round_count, global_threshold, src_id, dst_id,
                     vector<double>{1.0}, constructors, connections) {
    int x = 0;
    x++;
}

pair<dynamic_bitset<>, double> CipherAnalyzer::get_next_differential() {
    while (curr_round_idx < rounds.size()) {
        if (!advance_state()) {
            return {dynamic_bitset<>(0), 0.0};
        }
    }
    curr_round_idx--;
    global_threshold = max(global_threshold, round_probabilities.back());
    return {rounds.back()->dst->get_output(), round_probabilities.back()};
}

void CipherAnalyzer::set_input(const dynamic_bitset<> &bits, bits_range range) {
    rounds[0]->set_input(bits, range);
    rounds[0]->set_threshold(global_threshold /
                             optimal_probabilities[rounds.size() - 1]);
    curr_round_idx = 0;
}
