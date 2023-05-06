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

class CipherAnalyzer {
  protected:
    /**
     * @brief global_thresh
     */
    double global_thresh;

    /**
     * @brief opt_probs
     */
    vector<double> opt_probs;

    /**
     * @brief round_probs
     */
    vector<double> round_probs;

    /**
     * @brief rounds
     */
    vector<RoundFunctionPtr> rounds;

    /**
     * @brief curr_idx
     */
    size_t curr_idx;

    /**
     * @brief advance_state
     * @return
     */
    bool advance_state();

  public:
    /**
     * @brief CipherAnalyzer
     * @param rounds
     * @param input_max_hamming_weight
     * @param global_thresh
     * @param opt_probs
     */
    CipherAnalyzer(vector<RoundFunctionPtr> rounds, size_t input_max_hamming_weight, double global_thresh,
                   vector<double> opt_probs);

    /**
     * @brief CipherAnalyzer
     * @param rounds
     * @param input_max_hamming_weight
     * @param global_thresh
     */
    CipherAnalyzer(vector<RoundFunctionPtr> rounds, size_t input_max_hamming_weight, double global_thresh);

    /**
     * @brief get_next_entry
     * @return
     */
    ProbEntry get_next_entry();

    /**
     * @brief set_input
     * @param bits
     * @param rng
     */
    void set_input(const dynamic_bitset<> &bits, BitsRange rng);
};

#endif // CIPHERANALYZER_H
