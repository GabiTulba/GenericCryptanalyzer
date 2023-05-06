#include "pbox.h"
#include <set>

PBox::PBox(size_t data_size, const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes, const vector<size_t> &bit_perm)
    : AbstractBitShiftBox(data_size, data_size, dst_boxes, bit_perm) {
    set<size_t> used_bits;
    for (auto &bit_src : bit_perm) {
        used_bits.insert(bit_src);
    }
    assert(used_bits.size() == data_size);
}

PBox::PBox(size_t data_size, const vector<size_t> &bit_perm) : AbstractBitShiftBox(data_size, data_size, bit_perm) {
    set<size_t> used_bits;
    for (auto &bit_src : bit_perm) {
        used_bits.insert(bit_src);
    }
    assert(used_bits.size() == data_size);
}