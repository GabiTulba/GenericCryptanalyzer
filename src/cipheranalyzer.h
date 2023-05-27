/** @file cipheranalyzer.h
 *  @brief implementation of the CipherAnalyzer class
 */
#ifndef CIPHERANALYZER_H
#define CIPHERANALYZER_H

#include "roundfunction.h"

class CipherAnalyzer;

/**
 * @brief shorthand for std::shared_ptr<CipherAnalyzer>
 */
typedef std::shared_ptr<CipherAnalyzer> CipherAnalyzerPtr;

/**
 * @brief a CipherAnalyzer is a collection of rounds of a cipher that are linked togheter linearly.
 */
class CipherAnalyzer {
  protected:
    /**
     * @brief the minimum probability threshold such that the cipher outputs a state.
     */
    double global_thresh;

    /**
     * @brief optimal probabilities for all previous rounds
     */
    vector<double> opt_probs;

    /**
     * @brief a collection of all rounds that form this cipher.
     */
    vector<RoundFunctionPtr> rounds;

    /**
     * @brief the index in `rounds` of the current round in the search algorithm
     */
    size_t curr_idx;

    /**
     * @brief `vector` of all previous partial probabilities up to curr_idx. Used in calculating the final
     * probability of the cipher.
     */
    vector<double> round_probs;

    /**
     * @brief advances to the next internal state state of the cipher
     * @return false if there is no next state, otherwise returns true
     */
    bool advance_state();

  public:
    /**
     * @param a `vector` of rounds that represents the rounds of the cipher, the output of of each round is set as the
     * input of the next one
     * @param input_max_hamming_weight the maximum hamming weight when searching the optimal probabilities of ciphers
     * with less rounds
     * @param global_thresh the minimum global threshold probability that an output state must have in order to be
     * returned
     * @param opt_probs, if known, optimal probabilities of all ciphers with less rounds than this one
     */
    CipherAnalyzer(vector<RoundFunctionPtr> rounds, size_t input_max_hamming_weight, double global_thresh,
                   vector<double> opt_probs);

    /**
     * @param a `vector` of rounds that represents the rounds of the cipher, the output of of each round is set as the
     * input of the next one
     * @param input_max_hamming_weight the maximum hamming weight when searching the optimal probabilities of ciphers
     * with less rounds
     * @param global_thresh the minimum global threshold probability that an output state must have in order to be
     * returned
     */
    CipherAnalyzer(vector<RoundFunctionPtr> rounds, size_t input_max_hamming_weight, double global_thresh);

    /**
     * @brief returns the next possible output state that has a probability greater than `global_thresh`. Also sets
     * `global_thresh` to the new best probability in order to minimize the size of the search tree
     * @return the next computed output state, otherwise `{dynamic_bitset<>(0), 0.0}`
     */
    ProbEntry get_next_state();

    /**
     * @brief sets a subrange `rng` of the input to the value of `bits`
     * @param bits the bits that will be put in the `in_bits` of `src` of the first round
     * @param rng the subrange in which bits will be put in `in_bits` of `src` of the first round
     */
    void set_input(const dynamic_bitset<> &bits, BitsRange rng);
};

#endif // CIPHERANALYZER_H
