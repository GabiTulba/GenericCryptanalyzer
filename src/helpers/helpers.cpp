#include "helpers.h"

ProbTable compute_diff_dist_table(const vector<size_t> &sbox) noexcept(false) {
    size_t input_size = sbox.size();
    size_t output_size = *max_element(sbox.begin(), sbox.end()) + 1;
    if (popcnt(input_size) != 1) {
        throw std::logic_error("sbox input size must be a power of 2");
    }
    if (popcnt(output_size) != 1) {
        throw std::logic_error("sbox output size must be a power of 2");
    }
    size_t output_size_bits = popcnt(output_size - 1);

    ProbTable table(input_size);
    vector<vector<double>> probability_matrix(input_size, vector<double>(output_size, 0.0));

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
                    {to_dynamic_bitset(output_diff, output_size_bits), probability_matrix[input_diff][output_diff]});
                ;
            }
        }
        std::sort(table[input_diff].begin(), table[input_diff].end(),
                  [](const pair<dynamic_bitset<>, double> &a, const pair<dynamic_bitset<>, double> &b) -> bool {
                      return a.second > b.second;
                  });
    }

    return table;
}

bool increment_bits(dynamic_bitset<> &input) {
    if (input.count() == 0) {
        return false;
    }
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

dynamic_bitset<> to_dynamic_bitset(unsigned int input, size_t bit_size) {
    dynamic_bitset<> result(bit_size);
    size_t idx = 0;
    while (input) {
        result[idx++] = input & 1;
        input >>= 1;
    }

    return result;
}

size_t convert_to_size_t(const dynamic_bitset<> &bits) {
    size_t ans = 0;
    for (ssize_t i = bits.size() - 1; i >= 0; i--) {
        ans = (ans << 1) + bits[i];
    }
    return ans;
}

unsigned int convert_to_uint(const dynamic_bitset<> &bits) {
    int ans = 0;
    for (ssize_t i = bits.size() - 1; i >= 0; i--) {
        ans = (ans << 1) + bits[i];
    }
    return ans;
}

size_t popcnt(unsigned int x) {
    const uint32_t m1 = 0x55555555;
    const uint32_t m2 = 0x33333333;
    const uint32_t m4 = 0x0f0f0f0f;
    x -= (x >> 1) & m1;
    x = (x & m2) + ((x >> 2) & m2);
    x = (x + (x >> 4)) & m4;
    x += x >> 8;
    return (x + (x >> 16)) & 0x3f;
}
