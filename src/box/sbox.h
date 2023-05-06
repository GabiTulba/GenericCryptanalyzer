#ifndef SBOX_H
#define SBOX_H

#include "abstractbox.h"
#include "src/helpers/helpers.h"

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
    ProbTable prob_table;

    /**
     * @brief table_idx the current column in the row of the probability table. This is incremented each time
     * `determine_next()` is called.
     */
    size_t table_idx;

    /**
     * @brief table_entry the row in the probability table corresponding to the value of `in_bits`
     */
    size_t table_entry;

  public:
    /**
     * @param in_size size of the input bits of this box
     * @param out_size size of the output bits of this box
     * @param dst_boxes output flow connections to following boxes
     * @param prob_table the probability table of the sbox
     */
    SBox(size_t in_size, size_t out_size, const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes,
         const ProbTable &prob_table);

    /**
     * @brief similar to the previous constructor, but leaves `dst_boxes` empty
     * @param in_size size of the input bits of this box
     * @param out_size size of the output bits of this box
     * @param prob_table the probability table of the sbox
     */
    SBox(size_t in_size, size_t out_size, const ProbTable &prob_table);

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
