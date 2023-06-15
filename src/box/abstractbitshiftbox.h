/** @file abstractbitshiftbox.h
 *  @brief implementation of the AbstractBitShiftBox class
 */
#ifndef ABSTRACTBITSHIFTBOX_H
#define ABSTRACTBITSHIFTBOX_H

#include "abstractbox.h"

/**
 * @brief An AbstractBitShiftBox represents a generic box that transforms input in output by shifting bits around
 *
 * More precisely, it takes a `vector<size_t> bit_src` as input and computes `out_bits[i] = in_bits[bit_src[i]]`
 *
 * This class is an abstraction that encapsulates the functionality of both PBox and EBox.
 *
 * @see PBox
 * @see EBox
 *
 * @inherits AbstractBox
 */
class AbstractBitShiftBox : public AbstractBox {
  protected:
    /**
     * @brief an array which describes what bit from the input corresponds to each bit from the output
     */
    vector<size_t> bit_src;

    /**
     * @brief computes value of `out_bits` from `in_bits` and `bit_src`
     */
    void apply_transformation();

    /**
     * @brief get_best_prob return the best probability of an output for the box, for the AbstractBitShiftBox class,
     * this is always 1.0
     * @return a `double` with the value of 1.0
     */
    double get_best_prob() override;

  public:
    /**
     * @param in_size size of the input bits of this box
     * @param out_size size of the output bits of this box
     * @param bit_src an array used to compute `out_bits` from `in_bits`
     *
     * @pre bit_source.size() == output_size
     *
     * @throw if the precondition above is not fulfilled
     */
    AbstractBitShiftBox(size_t in_size, size_t out_size, const vector<size_t> &bit_src) noexcept(false);

    /**
     * @brief since apply_transformation is a linear deterministic transformations, this will set
     * `is_det` to `true` and `prob` to `1`
     */
    void determine_next() override;
};

/**
 * @brief shorthand for `std::shared_ptr<AbstractBitShiftBox>`
 */
typedef std::shared_ptr<AbstractBitShiftBox> AbstractBitShiftBoxPtr;

#endif // ABSTRACTBITSHIFTBOX_H
