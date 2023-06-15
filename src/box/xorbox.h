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
  protected:
    /**
     * @brief get_best_prob return the best probability of an output for the box, for the XorBox class, this is
     * always 1.0
     * @return a `double` with the value of 1.0
     */
    double get_best_prob() override;

  public:
    /**
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
