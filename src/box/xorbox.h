/** @file xorbox.h
 *  @brief implementation of the XorBox class
 */
#ifndef XORBOX_H
#define XORBOX_H

#include "abstractbox.h"

/**
 * @brief A XorBox is a box that computes the bitwise xor of two inputs. In order to simplify the implementation the the
 * XorBox takes only one input `in_bits` but of size double of that of `out_bits`. the first half of `in_bits`
 * represents the first of the two inputs, and the second half represents the last of the two inputs.
 *
 * @inherits AbstractBox
 */
class XorBox : public AbstractBox {
  public:
    /**
     * @param data_size the size in bits of the input and the output
     * @param dst_boxes output flow connections to following boxes
     *
     * @throw if the preconditions of `AbstractBox` for `dst_boxes` are not fulfilled
     */
    XorBox(size_t data_size, const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes) noexcept(false);
    /**
     * @brief similar to the previous constructor, but leaves `dst_boxes` empty
     * @param data_size the size in bits of the input and the output
     */
    XorBox(size_t data_size);

    /**
     * @brief computes the xor between the first half and the second half of the `in_bits` and stores it in `out_bits`.
     * This is a linear and deterministic operation so`is_det` will be set to `true` and `prob` to `1`
     */
    void determine_next() override;
};

/**
 * @brief shorthand for std::shared_ptr<XorBox>
 */
typedef std::shared_ptr<XorBox> XorBoxPtr;

#endif // XORBOX_H
