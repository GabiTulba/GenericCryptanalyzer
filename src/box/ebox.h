#ifndef EBOX_H
#define EBOX_H

#include "abstractbitshiftbox.h"

/**
 * @brief An EBox is a box that takes the input bits and expands them to the output by using some of multiple times
 *
 * @inherits AbstractBitShiftBox
 */
class EBox : public AbstractBitShiftBox {
  public:
    /**
     * @param in_size size of the input bits of this box
     * @param out_size size of the output bits of this box
     * @param dst_boxes output flow connections to following boxes
     * @param bit_expansion an array used to compute to describe the expansion process of the input bits
     *
     * @see AbstractShiftBox constructor
     * @pre in_size <= out_size
     */
    EBox(size_t in_size, size_t out_size, const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes,
         const vector<size_t> &bit_expansion);
    /**
     * @brief similar to the previous constructor, but leaves `dst_boxes` empty
     * @param in_size size of the input bits of this box
     * @param out_size size of the output bits of this box
     * @param bit_expansion an array used to compute to describe the expansion process of the input bits
     *
     * @pre in_size <= out_size
     */
    EBox(size_t in_size, size_t out_size, const vector<size_t> &bit_expansion);
};

/**
 * @brief shorthand for std::shared_ptr<Ebox>
 */
typedef std::shared_ptr<EBox> EBoxPtr;

#endif // EBOX_H
