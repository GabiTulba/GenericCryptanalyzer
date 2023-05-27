/** @file identitybox.h
 *  @brief implementation of the IdentityBox class
 */
#ifndef IDENTITYBOX_H
#define IDENTITYBOX_H

#include "abstractbox.h"

/**
 * @brief An IdentityBox is a box that represents the identity function `out_bits[i] = in_bits[i]`. An IdentityBox is
 * useful as an accumulator for bits from multiple previous boxes which will then be sent to multiple following boxes
 *
 * @inherits AbstractBox
 */
class IdentityBox : public AbstractBox {
  public:
    /**
     * @param data_size the size in bits of the input and the output
     * @param dst_boxes output flow connections to following boxes
     *
     * @throw if the preconditions of `AbstractBox` for `dst_boxes` are not fulfilled
     */
    IdentityBox(size_t data_size, const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes) noexcept(false);
    /**
     * @brief similar to the previous constructor, but leaves `dst_boxes` empty
     * @param data_size the size in bits of the input and the output
     */
    IdentityBox(size_t data_size);

    /**
     * @brief since the identity function is a linear deterministic transformations, this will set `is_det` to `true`
     * and `prob` to `1`
     */
    void determine_next() override;
};

/**
 * @brief shorthand for std::shared_ptr<IdentityBox>
 */
typedef std::shared_ptr<IdentityBox> IdentityBoxPtr;

#endif // IDENTITYBOX_H
