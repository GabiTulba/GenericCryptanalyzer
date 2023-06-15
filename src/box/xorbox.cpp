#include "xorbox.h"

double XorBox::get_best_prob() { return 1.0; }

XorBox::XorBox(size_t data_size) : AbstractBox(data_size * 2, data_size) {}

void XorBox::determine_next() {
    if (!is_det) {
        auto in_bits1 = (in_bits >> out_bits.size());
        in_bits1.resize(out_bits.size());
        auto in_bits2 = in_bits;
        in_bits2.resize(out_bits.size());
        out_bits = in_bits1 ^ in_bits2;
        is_det = true;
        prob = 1.0;
    }
}
