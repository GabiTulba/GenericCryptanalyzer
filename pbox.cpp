#include "pbox.h"
#include <set>

PBox::PBox(size_t bits_size,
           const vector<pair<std::shared_ptr<AbstractBox>,
                             pair<bits_range, bits_range>>> &dest_boxes,
           const vector<size_t> &bit_permutation)
    : AbstractBitShiftBox(bits_size, bits_size, dest_boxes, bit_permutation) {
    set<size_t> used_bits;
    for (auto &bit_source : bit_permutation) {
        used_bits.insert(bit_source);
    }
    assert(used_bits.size() == bits_size);
}

PBox::PBox(size_t bits_size, const vector<size_t> &bit_permutation)
    : AbstractBitShiftBox(bits_size, bits_size, bit_permutation) {
    set<size_t> used_bits;
    for (auto &bit_source : bit_permutation) {
        used_bits.insert(bit_source);
    }
    assert(used_bits.size() == bits_size);
}
