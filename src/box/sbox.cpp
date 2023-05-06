#include "sbox.h"

using namespace std;

SBox::SBox(size_t in_size, size_t out_size, const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes,
           const ProbTable &prob_table)
    : AbstractBox(in_size, out_size, dst_boxes), prob_table(prob_table), table_idx(0) {}

SBox::SBox(size_t in_size, size_t out_size, const ProbTable &prob_table)
    : AbstractBox(in_size, out_size), prob_table(prob_table), table_idx(0) {}

void SBox::determine_next() {
    if (table_idx < prob_table[table_entry].size()) {
        out_bits = prob_table[table_entry][table_idx].first;
        prob = prob_table[table_entry][table_idx].second;
        table_idx++;
    }
    if (table_idx == prob_table[table_entry].size()) {
        is_det = true;
    }
}

void SBox::reset_determination() {
    AbstractBox::reset_determination();
    table_idx = 0;
}

void SBox::set_input(dynamic_bitset<> bits, const BitsRange &rng) {
    AbstractBox::set_input(bits, rng);
    table_idx = 0;
    table_entry = convert_to_index(in_bits);
}
