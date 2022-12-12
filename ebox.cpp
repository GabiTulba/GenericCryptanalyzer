#include "ebox.h"

EBox::EBox(size_t input_size, size_t output_size,
           const vector<pair<std::shared_ptr<AbstractBox>,
                             pair<bits_range, bits_range>>> &dest_boxes,
           const vector<size_t> &bit_expansion)
    : AbstractBitShiftBox(input_size, output_size, dest_boxes, bit_expansion) {
    assert(input_size <= output_size);
}

EBox::EBox(size_t input_size, size_t output_size,
           const vector<size_t> &bit_expansion)
    : AbstractBitShiftBox(input_size, output_size, bit_expansion) {
    assert(input_size <= output_size);
}
