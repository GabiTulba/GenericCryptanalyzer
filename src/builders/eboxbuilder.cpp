#include "eboxbuilder.h"

EBoxBuilder::EBoxBuilder(vector<size_t> bit_expansion) : bit_expansion(bit_expansion) {}

AbstractBoxPtr EBoxBuilder::build() {
    size_t bit_size_in = *max_element(bit_expansion.begin(), bit_expansion.end()) + 1;
    size_t bit_size_out = bit_expansion.size();
    return std::make_shared<EBox>(bit_size_in, bit_size_out, bit_expansion);
}

AbstractBoxBuilderPtr make_ebox_builder(vector<size_t> bit_expansion) {
    return std::make_shared<EBoxBuilder>(bit_expansion);
}
