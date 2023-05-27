#include "ebox.h"

EBox::EBox(size_t in_size, size_t out_size, const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes,
           const vector<size_t> &bit_expansion) noexcept(false)
    : AbstractBitShiftBox(in_size, out_size, dst_boxes, bit_expansion) {
    if (bit_expansion.size() != in_size) {
        throw std::logic_error("input size must be equal with bit_expansion' size");
    }
    if (*max_element(bit_expansion.begin(), bit_expansion.end()) + 1 != out_size) {
        throw std::logic_error("output size must be equal with bit_expansion's max element + 1");
    }

    if (in_size > out_size) {
        throw std::logic_error("output size must be at least as big as input size");
    }
}

EBox::EBox(size_t in_size, size_t out_size, const vector<size_t> &bit_expansion) noexcept(false)
    : AbstractBitShiftBox(in_size, out_size, bit_expansion) {
    if (in_size > out_size) {
        throw std::logic_error("output size must be at least as big as input size");
    }
}
