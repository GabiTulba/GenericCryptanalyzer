#include "ebox.h"

EBox::EBox(size_t in_size, size_t out_size,
           const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes,
           const vector<size_t> &bit_expansion)
    : AbstractBitShiftBox(in_size, out_size, dst_boxes, bit_expansion) {
    assert(in_size <= out_size);
}

EBox::EBox(size_t in_size, size_t out_size, const vector<size_t> &bit_expansion)
    : AbstractBitShiftBox(in_size, out_size, bit_expansion) {
    assert(in_size <= out_size);
}
