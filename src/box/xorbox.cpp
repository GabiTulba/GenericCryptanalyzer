#include "xorbox.h"

XorBox::XorBox(size_t data_size, const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes)
    : AbstractBox(data_size * 2, data_size, dst_boxes) {}

XorBox::XorBox(size_t data_size) : AbstractBox(data_size * 2, data_size) {}

void XorBox::determine_next() {
    auto in_bits1 = (in_bits >> 32);
    in_bits1.resize(out_bits.size());
    auto in_bits2 = in_bits;
    in_bits2.resize(out_bits.size());
    out_bits = in_bits1 ^ in_bits2;
    is_det = true;
    prob = 1.0;
}
