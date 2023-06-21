#include "sbox.h"

using namespace std;

double SBox::get_best_prob() {
    if (prob_table->find(in_bits) == prob_table->end()) {
        return 0;
    }
    return (*prob_table)[in_bits][0].second;
}

SBox::SBox(size_t in_size, size_t out_size, ProbTablePtr prob_table, bool is_exhaustive) noexcept(false)
    : AbstractBox(in_size, out_size), prob_table(prob_table), table_idx(0), is_exhaustive(is_exhaustive) {
    if (in_size != prob_table->begin()->first.size()) {
        throw std::logic_error("input size must be equal with prob_table's input size");
    }

    // It is guaranteed that if the input difference is 0, there is a 0 output difference with probability 1
    in_bits.set(0, in_bits.size(), 0);
    if (out_size != (*prob_table)[in_bits][0].first.size()) {
        throw std::logic_error("output size must be equal with prob_table's element size");
    }
}

void SBox::determine_next() {
    auto table_row = (*prob_table)[in_bits];
    if ((table_idx < table_row.size() && is_exhaustive) || table_idx == 0) {
        out_bits = table_row[table_idx].first;
        prob = table_row[table_idx].second;
        table_idx++;
    }
    if (table_idx == table_row.size() || !is_exhaustive) {
        is_det = true;
    }
}

void SBox::reset_determination() {
    AbstractBox::reset_determination();
    table_idx = 0;
}

void SBox::set_input(dynamic_bitset<> bits, const BitsRange &rng) {
    AbstractBox::set_input(bits, rng);
    if (prob_table->find(in_bits) == prob_table->end()) {
        is_det = true;
    }
    table_idx = 0;
}
