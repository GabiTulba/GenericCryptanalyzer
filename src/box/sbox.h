/** @file sbox.h
 *  @brief implementation of the SBox class
 */
#ifndef SBOX_H
#define SBOX_H

#include "abstractbox.h"

/**
 * @brief An SBox is a box that applies an arbitrary substitution on the input based on a substitution table. Since the
 * actual value of the difference between pairs of inputs will change depending on the key bytes, this is a
 * non-deterministic element in the cipher.
 *
 * @inherits AbstractBox
 */
class SBox : public AbstractBox {

  protected:
    /**
     * @brief prob_table the probability table of the sbox
     */
    ProbTablePtr prob_table;

    /**
     * @brief table_idx the current column in the row of the probability table. This is incremented each time
     * `determine_next()` is called.
     */
    size_t table_idx;

    /**
     * @brief is_exhaustive a `bool` that represents if the sbox will generate all possible outputs from the set input,
     * or if false, it will generate only the best output. Used for optimizing out branches of the search algorithm.
     */
    bool is_exhaustive;

    /**
     * @brief get_best_prob return the best probability of an output for the box, for the SBox class, this is the
     * probability of the first entry in the current row of the probability table
     * @return a `double` with the value of the best probability for the set input
     */
    double get_best_prob() override;

  public:
    /**
     * @param in_size size of the input bits of this box
     * @param out_size size of the output bits of this box
     * @param prob_table the probability table of the sbox
     * @param is_exhaustive a boolean that if set to true will set the sbox to go through all the possible output of the
     * sbox, otherwise will only go through the best output
     *
     * @pre `prob_table`'s size is a power of 2
     * @pre `prob_table`'s max element is one less than a power of 2
     *
     * @throw if the preconditions above are not fulfilled
     */
    SBox(size_t in_size, size_t out_size, ProbTablePtr prob_table, bool is_exhaustive) noexcept(false);

    /**
     * @brief determines the next best possible output for the given input, if all possible outputs have been
     * determined, `is_det` is set to `true`
     */
    void determine_next() override;

    /**
     * @brief sets `prob` to `0`, `is_det` to `false` and `table_idx` to `0`
     */
    void reset_determination() override;

    /**
     * @brief sets a subrange `rng` of the input to the value of `bits`. Additionaly, `table_idx` to `0` and computes
     * `table_entry` from `in_bits`
     * @param bits the bits that will be put in `in_bits`
     * @param rng the subrange in which bits will be put in `in_bits`
     */
    void set_input(dynamic_bitset<> bits, const BitsRange &rng) override;
};
/**
 * @brief shorthand for std::shared_ptr<SBox>
 */
typedef std::shared_ptr<SBox> SBoxPtr;

#endif // SBOX_H
