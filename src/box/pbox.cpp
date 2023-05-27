#include "pbox.h"
#include <set>

PBox::PBox(size_t data_size, const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes,
           const vector<size_t> &bit_perm) noexcept(false)
    : AbstractBitShiftBox(data_size, data_size, dst_boxes, bit_perm) {
    set<size_t> used_bits;
    for (auto &bit_src : bit_perm) {
        used_bits.insert(bit_src);
    }
    if (used_bits.size() != data_size || *used_bits.begin() != 0 || *used_bits.rbegin() != data_size - 1) {
        throw std::logic_error("bit_perm must be a permutation of [0..., data_size - 1]");
    }
}

PBox::PBox(size_t data_size, const vector<size_t> &bit_perm) noexcept(false)
    : AbstractBitShiftBox(data_size, data_size, bit_perm) {
    set<size_t> used_bits;
    for (auto &bit_src : bit_perm) {
        used_bits.insert(bit_src);
    }
    if (used_bits.size() != data_size || *used_bits.begin() != 0 || *used_bits.rbegin() != data_size - 1) {
        throw std::logic_error("bit_perm must be a permutation of [0..., data_size - 1]");
    }
}
