/** @file abstractbox.h
 *  @brief implementation of the AbstractBox class
 */
#ifndef ABSTRACTBOX_H
#define ABSTRACTBOX_H

#include "helpers/helpers.h"

using namespace std;
using namespace boost;

class AbstractBox;

/**
 * @brief shorthand for `std::shared_ptr<AbstractBox>`
 */
typedef std::shared_ptr<AbstractBox> AbstractBoxPtr;

/**
 * @brief An AbstractBox represents an abstract idea of a block cipher component such as a Pbox, Sbox, Xor, Addition,
 * etc. A cipher is composed of multiple such `boxes` that communicate with each other through connections
 */
class AbstractBox {
    friend class RoundFunction;

  protected:
    /**
     * @brief the bits that flow into the box
     */
    dynamic_bitset<> in_bits;

    /**
     * @brief the bits that flow out of the box
     */
    dynamic_bitset<> out_bits;

    /**
     * @brief describes where bits from previous boxes flow into the `in_bits` of this box
     */
    vector<pair<AbstractBoxPtr, Connection>> src_boxes;

    /**
     * @brief describes how the `out_bits` flow from this box to other following boxes
     */
    vector<pair<AbstractBoxPtr, Connection>> dst_boxes;

    /**
     * @brief a boolean value that should be true if and only if all possible outputs have been determined and returned
     */
    bool is_det;

    /**
     * @brief the probability of the box to output the currently determined state
     */
    double prob;

    /**
     * @brief get_best_prob return the best probability of an output for the box
     * @return a `double` representing the best probability of an output
     */
    virtual double get_best_prob() = 0;

  public:
    /**
     * @param in_size size of the input bits of this box
     * @param out_size size of the output bits of this box
     */
    AbstractBox(size_t in_size, size_t out_size);

    /**
     * @brief add_dest adds a new destination box for the output of the box to flow to
     * @param dst_box a pointer to the destination box
     * @param out_range a subrange of `out_bits` from this box that will flow to `dst_box`
     * @param in_range a subrange of `in_bits` from `dst_box` into which the bits will flow
     *
     * @pre dst_box != nullptr && out_rng.start + out_rng.len <= out_bits.size() &&
     *      in_rng.start + in_rng.len <= dst_box->in_bits.size();
     *
     * @throw if the precondition above is not fulfilled

     */
    void add_dest(AbstractBoxPtr dst_box, BitsRange out_range, BitsRange in_range) noexcept(false);

    /**
     * @brief add_src adds a new source box for the input of the box
     * @param src_box a pointer to the source box
     * @param out_range a subrange of `out_bits` from `src_box` that will flow into this box
     * @param in_range a subrange of `in_bits` from this box into which the bits will flow
     *
     * @pre src_box != nullptr && out_rng.start + out_rng.len <= src_box->out_bits.size() &&
     *      in_rng.start + in_rng.len <= in_bits.size();
     *
     * @throw if the precondition above is not fulfilled

     */
    void add_src(AbstractBoxPtr src_box, BitsRange out_range, BitsRange in_range) noexcept(false);

    /**
     * @brief getter for `in_bits`
     * @return `in_bits`
     */
    const dynamic_bitset<> &get_input();

    /**
     * @brief getter for `out_bits`
     * @return `out_bits`
     */
    const dynamic_bitset<> &get_output();

    /**
     * @brief getter for the size of `in_bits`
     * @return `in_bits.size()`
     */
    size_t input_size();

    /**
     * @brief getter for the size of `out_bits`
     * @return `out_bits.size()`
     */
    size_t output_size();

    /**
     * @brief getter for `is_det`
     * @return `is_det`
     */
    bool is_determined();

    /**
     * @brief sets a subrange `rng` of the input to the value of `bits`
     * @param bits the bits that will be put in `in_bits`
     * @param rng the subrange in which bits will be put in `in_bits`
     */
    virtual void set_input(dynamic_bitset<> bits, const BitsRange &rng);

    /**
     * @brief notifies all the destination boxes after the output of the box is determined
     */
    void notify_all();

    /**
     * @brief method to determine the next best output sorted by probabilities, if all possible outputs have been
     * determined, `is_det` will be set to `true`
     */
    virtual void determine_next() = 0;

    /**
     * @brief set the process to be undetermined by setting `is_det` to `false`
     */
    virtual void reset_determination();

    /**
     * @brief get the probability of the current characteristic
     * @return `prob`
     */
    double get_probability();
};

#endif // ABSTRACTBOX_H
