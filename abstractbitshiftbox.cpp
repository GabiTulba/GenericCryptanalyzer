#include "abstractbitshiftbox.h"

void AbstractBitShiftBox::apply_transformation() {
    for (size_t i = 0; i < output_bits.size(); i++) {
        output_bits[i] = input_bits[bit_source[i]];
    }
}

AbstractBitShiftBox::AbstractBitShiftBox(
    size_t input_size, size_t output_size,
    const vector<pair<std::shared_ptr<AbstractBox>,
                      pair<bits_range, bits_range>>> &dest_boxes,
    const vector<size_t> &bit_source)
    : AbstractBox(input_size, output_size, dest_boxes), bit_source(bit_source) {
    assert(bit_source.size() == output_size);
}

AbstractBitShiftBox::AbstractBitShiftBox(size_t input_size, size_t output_size,
                                         const vector<size_t> &bit_source)
    : AbstractBox(input_size, output_size), bit_source(bit_source) {
    assert(bit_source.size() == output_size);
}

void AbstractBitShiftBox::determine_next() {
    apply_transformation();
    determined = true;
    probability = 1;
}
