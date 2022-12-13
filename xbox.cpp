#include "xbox.h"

XBox::XBox(size_t data_size,
           const vector<pair<std::shared_ptr<AbstractBox>,
                             pair<bits_range, bits_range>>> &dest_boxes)
    : AbstractBox(data_size * 2, data_size, dest_boxes) {}

XBox::XBox(size_t data_size) : AbstractBox(data_size * 2, data_size) {}

void XBox::determine_next() {
    auto input_bits1 = (input_bits >> 32);
    input_bits1.resize(output_bits.size());
    auto input_bits2 = input_bits;
    input_bits2.resize(output_bits.size());
    output_bits = input_bits1 ^ input_bits2;
    determined = true;
    probability = 1.0;
}
