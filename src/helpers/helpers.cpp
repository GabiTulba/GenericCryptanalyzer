#include "helpers.h"

const char *hex_chars = "0123456789abcdef";

ProbTablePtr compute_diff_dist_table(const vector<size_t> &sbox) noexcept(false) {
    size_t input_size = sbox.size();
    size_t output_size = *max_element(sbox.begin(), sbox.end()) + 1;
    if (popcnt(input_size) != 1) {
        throw std::logic_error("sbox input size must be a power of 2");
    }
    if (popcnt(output_size) != 1) {
        throw std::logic_error("sbox output size must be a power of 2");
    }
    size_t input_size_bits = popcnt(input_size - 1);
    size_t output_size_bits = popcnt(output_size - 1);

    ProbTablePtr table = std::make_shared<ProbTable>();
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
                (*table)[to_dynamic_bitset(input_diff, input_size_bits)].push_back(
                    {to_dynamic_bitset(output_diff, output_size_bits), probability_matrix[input_diff][output_diff]});
                ;
            }
        }
        std::sort((*table)[to_dynamic_bitset(input_diff, input_size_bits)].begin(),
                  (*table)[to_dynamic_bitset(input_diff, input_size_bits)].end(),
                  [](const pair<dynamic_bitset<>, double> &a, const pair<dynamic_bitset<>, double> &b) -> bool {
                      return a.second > b.second;
                  });
    }

    return table;
}

dynamic_bitset<> eq_func(dynamic_bitset<> &a, dynamic_bitset<> &b, dynamic_bitset<> &c) {
    return ((~a) ^ b) & ((~a) ^ c);
}

dynamic_bitset<> mask_except_msb(size_t bit_size) {
    dynamic_bitset<> result(bit_size);
    result.set(0, result.size(), 1);
    result[bit_size - 1] = 0;

    return result;
}

double get_xdpplus_prob(dynamic_bitset<> &a, dynamic_bitset<> &b, dynamic_bitset<> &c) {
    dynamic_bitset<> a_shift = a << 1;
    dynamic_bitset<> b_shift = b << 1;
    dynamic_bitset<> c_shift = c << 1;
    if ((eq_func(a_shift, b_shift, c_shift) & (a ^ b ^ c ^ b_shift)).count() != 0) {
        return 0.0;
    }
    return 1.0 / (1LL << (~eq_func(a, b, c) & mask_except_msb(a.size())).count());
}

void compute_pddt_helper(size_t bit_size, size_t curr_idx, dynamic_bitset<> a, dynamic_bitset<> b, dynamic_bitset<> c,
                         ProbTablePtr &pddt, double prob_thresh) {
    if (curr_idx == bit_size) {
        double diff_prob = get_xdpplus_prob(a, b, c);

        a.resize(2 * bit_size);
        b.resize(2 * bit_size);
        b <<= bit_size;
        (*pddt)[a | b].push_back({c, diff_prob});
        return;
    }

    a.push_back(0);
    b.push_back(0);
    c.push_back(0);

    for (size_t a_bit = 0; a_bit <= 1; a_bit++) {
        a[curr_idx] = a_bit;
        for (size_t b_bit = 0; b_bit <= 1; b_bit++) {
            b[curr_idx] = b_bit;
            for (size_t c_bit = 0; c_bit <= 1; c_bit++) {
                c[curr_idx] = c_bit;
                if (get_xdpplus_prob(a, b, c) >= prob_thresh) {
                    compute_pddt_helper(bit_size, curr_idx + 1, a, b, c, pddt, prob_thresh);
                }
            }
        }
    }
}

ProbTablePtr compute_xdpplus_pddt(size_t bit_size, double prob_thresh) {
    ProbTablePtr pddt = std::make_shared<ProbTable>();

    compute_pddt_helper(bit_size, 0, dynamic_bitset<>(0), dynamic_bitset<>(0), dynamic_bitset<>(0), pddt, prob_thresh);

    for (auto &[input, line] : *pddt) {
        std::sort(line.begin(), line.end(),
                  [](const pair<dynamic_bitset<>, double> &a, const pair<dynamic_bitset<>, double> &b) -> bool {
                      return a.second > b.second;
                  });
    }

    return pddt;
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

dynamic_bitset<> to_dynamic_bitset(unsigned long long input, size_t bit_size) {
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
    unsigned int ans = 0;
    for (ssize_t i = bits.size() - 1; i >= 0; i--) {
        ans = (ans << 1) + bits[i];
    }
    return ans;
}

unsigned long long convert_to_ull(const dynamic_bitset<> &bits) {
    unsigned long long ans = 0;
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

string convert_to_hex_string(const dynamic_bitset<> &bits) {
    string hex_str = "";
    for (int i = 0; i < bits.size(); i += 4) {
        unsigned int hex_digit = 0;
        for (int j = min((int)(bits.size() - i - 1), 3); j >= 0; j--) {
            hex_digit = (hex_digit << 1) + bits[i + j];
        }
        hex_str.insert(hex_str.begin(), hex_chars[hex_digit]);
    }

    return hex_str;
}
