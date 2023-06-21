#include "sboxbuilder.h"

SBoxBuilder::SBoxBuilder(size_t bits_in_size, size_t bits_out_size, ProbTablePtr prob_table, bool is_exhaustive)
    : bits_in_size(bits_in_size), bits_out_size(bits_out_size), prob_table(prob_table), is_exhaustive(is_exhaustive) {}

AbstractBoxPtr SBoxBuilder::build() {
    return std::make_shared<SBox>(bits_in_size, bits_out_size, prob_table, is_exhaustive);
}

AbstractBoxBuilderPtr make_sbox_builder(size_t bits_in_size, size_t bits_out_size, ProbTablePtr prob_table,
                                        bool is_exhaustive) {
    return std::make_shared<SBoxBuilder>(bits_in_size, bits_out_size, prob_table, is_exhaustive);
}
