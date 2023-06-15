#include "sbox.h"

using namespace std;

double SBox::get_best_prob() { return prob_table[table_entry][0].second; }

SBox::SBox(size_t in_size, size_t out_size, const ProbTable &prob_table, bool is_exhaustive) noexcept(false)
    : AbstractBox(in_size, out_size), prob_table(prob_table), table_idx(0), is_exhaustive(is_exhaustive) {
    if (in_size != popcnt(prob_table.size() - 1)) {
        throw std::logic_error("input size must be equal with prob_table's size");
    }

    // It is guaranteed that if the input difference is 0, there is a 0 output difference with probability 1
    if (out_size != prob_table[0][0].first.size()) {
        throw std::logic_error("output size must be equal with prob_table's element size");
    }
}

void SBox::determine_next() {
    if ((table_idx < prob_table[table_entry].size() && is_exhaustive) || table_idx == 0) {
        out_bits = prob_table[table_entry][table_idx].first;
        prob = prob_table[table_entry][table_idx].second;
        table_idx++;
    }
    if (table_idx == prob_table[table_entry].size() || !is_exhaustive) {
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
    table_entry = convert_to_size_t(in_bits);
}
