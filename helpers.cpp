#include "helpers.h"

vector<vector<pair<dynamic_bitset<>, double>>>
compute_differential_table(const vector<size_t> &sbox) {
    size_t input_size = sbox.size();
    size_t output_size = *max_element(sbox.begin(), sbox.end()) + 1;
    assert(__builtin_popcount(input_size) == 1);
    assert(__builtin_popcount(output_size) == 1);

    vector<vector<pair<dynamic_bitset<>, double>>> table(input_size);
    vector<vector<double>> probability_matrix(input_size,
                                              vector<double>(output_size, 0.0));

    for (size_t i = 0; i < input_size; i++) {
        for (size_t j = 0; j < input_size; j++) {
            int input_diff = i ^ j;
            int output_diff = sbox[i] ^ sbox[j];

            probability_matrix[input_diff][output_diff] += 1;
        }
    }

    for (size_t input_diff = 0; input_diff < input_size; input_diff++) {
        for (size_t output_diff = 0; output_diff < output_size; output_diff++) {
            probability_matrix[input_diff][output_diff] /= input_size;
            if (probability_matrix[input_diff][output_diff] != 0) {
                table[input_diff].push_back(
                    {to_dynamic_bitset(output_diff, output_size),
                     probability_matrix[input_diff][output_diff]});
                ;
            }
        }
        std::sort(table[input_diff].begin(), table[input_diff].end(),
                  [](const pair<dynamic_bitset<>, double> &a,
                     const pair<dynamic_bitset<>, double> &b) -> bool {
                      return a.second > b.second;
                  });
    }

    return table;
}

bool increment_bits(dynamic_bitset<> &input) {
    size_t idx = input.find_first();
    size_t carry_cnt = 0;
    while (idx < input.size() && input[idx] == 1) {
        input[idx++] = 0;
        carry_cnt++;
    }

    if (idx == input.size()) {
        return false;
    }
    input[idx] = 1;
    input.set(0, carry_cnt - 1, 1);
    return true;
}

dynamic_bitset<> to_dynamic_bitset(size_t input, size_t bit_size) {
    dynamic_bitset<> result(bit_size);
    size_t idx = 0;
    while (input) {
        result[idx++] = input & 1;
        input >>= 1;
    }

    return result;
}
