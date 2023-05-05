#include "abstractbitshiftbox.h"

void AbstractBitShiftBox::apply_transformation() {
    for (size_t i = 0; i < out_bits.size(); i++) {
        out_bits[i] = in_bits[bit_src[i]];
    }
}

AbstractBitShiftBox::AbstractBitShiftBox(
    size_t input_size, size_t output_size,
    const vector<pair<AbstractBoxPtr, Connection>> &dest_boxes,
    const vector<size_t> &bit_source)
    : AbstractBox(input_size, output_size, dest_boxes), bit_src(bit_source) {
    assert(bit_source.size() == output_size);
}

AbstractBitShiftBox::AbstractBitShiftBox(size_t input_size, size_t output_size,
                                         const vector<size_t> &bit_source)
    : AbstractBox(input_size, output_size), bit_src(bit_source) {
    assert(bit_source.size() == output_size);
}

void AbstractBitShiftBox::determine_next() {
    apply_transformation();
    is_det = true;
    prob = 1;
}
