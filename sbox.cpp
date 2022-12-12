#include "sbox.h"

using namespace std;

size_t SBox::convert_to_index(const dynamic_bitset<> &bits) {
    size_t ans = 0;
    for (ssize_t i = bits.size() - 1; i >= 0; i--) {
        ans = (ans << 1) + bits[i];
    }
    return ans;
}

SBox::SBox(
    size_t input_size, size_t output_size,
    const vector<pair<std::shared_ptr<AbstractBox>,
                      pair<bits_range, bits_range>>> &dest_boxes,
    const vector<vector<pair<dynamic_bitset<>, double>>> &differential_table)
    : AbstractBox(input_size, output_size, dest_boxes),
      differential_table(differential_table), table_index(0) {}

SBox::SBox(
    size_t input_size, size_t output_size,
    const vector<vector<pair<dynamic_bitset<>, double>>> &differential_table)
    : AbstractBox(input_size, output_size),
      differential_table(differential_table), table_index(0) {}

void SBox::determine_next() {
    if (table_index < differential_table[table_entry].size()) {
        output_bits = differential_table[table_entry][table_index].first;
        probability = differential_table[table_entry][table_index].second;
        table_index++;
    }
    if (table_index == differential_table[table_entry].size()) {
        determined = true;
    }
}

void SBox::reset_determination() {
    AbstractBox::reset_determination();
    table_index = 0;
}

void SBox::set_input(dynamic_bitset<> bits, const bits_range &range) {
    AbstractBox::set_input(bits, range);
    table_index = 0;
    table_entry = convert_to_index(input_bits);
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
