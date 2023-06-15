#include "abstractbitshiftbox.h"

void AbstractBitShiftBox::apply_transformation() {
    for (size_t i = 0; i < out_bits.size(); i++) {
        out_bits[i] = in_bits[bit_src[i]];
    }
}

double AbstractBitShiftBox::get_best_prob() { return 1.0; }

AbstractBitShiftBox::AbstractBitShiftBox(size_t input_size, size_t output_size,
                                         const vector<size_t> &bit_source) noexcept(false)
    : AbstractBox(input_size, output_size), bit_src(bit_source) {
    if (bit_source.size() != output_size) {
        throw std::logic_error("input and output sizes must be equal");
    }
}

void AbstractBitShiftBox::determine_next() {
    if (!is_det) {
        apply_transformation();
        is_det = true;
        prob = 1;
    }
}
