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
     * @pre bit_perm must be a permutation of [0, ..., bits_size - 1]
     */
    PBox(size_t data_size, const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes, const vector<size_t> &bit_perm);
    /**
     * @brief PBox similar to the previous constructor, but leaves `dst_boxes` empty
     * @param data_size the size in bits of the input and the output
     * @param bit_perm a permutation used to shuffle the bits of the input to get the output
     *
     * @pre bit_perm must be a permutation of [0, ..., bits_size - 1]
     */
    PBox(size_t data_size, const vector<size_t> &bit_perm);
};

/**
 * @brief shorthand for std::shared_ptr<PBox>
 */
typedef std::shared_ptr<PBox> PBoxPtr;

#endif // PBOX_H
