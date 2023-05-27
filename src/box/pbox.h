/** @file pbox.h
 *  @brief implementation of the PBox class
 */
#ifndef PBOX_H
#define PBOX_H

#include "abstractbitshiftbox.h"

/**
 * @brief A PBox is a box that takes the input bits and permutes them to get the output
 *
 * @inherits AbstractBitShiftBox
 */
class PBox : public AbstractBitShiftBox {
  public:
    /**
     * @param data_size the size in bits of the input and the output
     * @param dst_boxes output flow connections to following boxes
     * @param bit_perm a permutation used to shuffle the bits of the input to get the output
     *
     * @see AbstractShiftBox constructor
     *
     * @pre bit_perm must be a permutation of `[0, ..., bits_size - 1]`
     *
     * @throw if the preconditions of `AbstractBox` for `dst_boxes` are not fulfilled or if the precondition of
     * `AbstractBitShiftBox` for `out_size` is not fufilled or if the precondition above is not fulfilled
     */
    PBox(size_t data_size, const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes,
         const vector<size_t> &bit_perm) noexcept(false);
    /**
     * @brief PBox similar to the previous constructor, but leaves `dst_boxes` empty
     * @param data_size the size in bits of the input and the output
     * @param bit_perm a permutation used to shuffle the bits of the input to get the output
     *
     * @see AbstractShiftBox constructor

     * @pre bit_perm must be a permutation of [0, ..., bits_size - 1]
     *
     * @throw if the precondition of `AbstractBitShiftBox` for `out_size` is not fufilled or if the precondition above
     * is not fulfilled
     */
    PBox(size_t data_size, const vector<size_t> &bit_perm) noexcept(false);
};

/**
 * @brief shorthand for std::shared_ptr<PBox>
 */
typedef std::shared_ptr<PBox> PBoxPtr;

#endif // PBOX_H
