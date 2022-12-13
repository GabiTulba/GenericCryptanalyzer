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
