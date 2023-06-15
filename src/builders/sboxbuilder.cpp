#include "sboxbuilder.h"

SBoxBuilder::SBoxBuilder(vector<size_t> sbox, bool is_exhaustive) : sbox(sbox), is_exhaustive(is_exhaustive) {}

AbstractBoxPtr SBoxBuilder::build() {
    size_t bits_in_size = popcnt(sbox.size() - 1);
    size_t bits_out_size = popcnt(*max_element(sbox.begin(), sbox.end()));
    return std::make_shared<SBox>(bits_in_size, bits_out_size, compute_diff_dist_table(sbox), is_exhaustive);
}

AbstractBoxBuilderPtr make_sbox_builder(vector<size_t> sbox, bool is_exhaustive) {
    return std::make_shared<SBoxBuilder>(sbox, is_exhaustive);
}
