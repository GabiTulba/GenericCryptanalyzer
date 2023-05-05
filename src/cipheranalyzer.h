#ifndef CIPHERANALYZER_H
#define CIPHERANALYZER_H

#include "helpers/helpers.h"
#include "roundfunction.h"

class CipherAnalyzer;

typedef std::shared_ptr<CipherAnalyzer> CipherAnalyzerPtr;

class CipherAnalyzer {
  protected:
    double global_thresh;
    vector<double> opt_probs;
    vector<double> round_probs;
    vector<RoundFunctionPtr> rounds;
    size_t curr_idx;

    bool advance_state();

  public:
    CipherAnalyzer(vector<RoundFunctionPtr> rounds,
                   size_t input_max_hamming_weight, double global_thresh,
                   vector<double> opt_probs);

    CipherAnalyzer(vector<RoundFunctionPtr> rounds,
                   size_t input_max_hamming_weight, double global_thresh);

    ProbEntry get_next_entry();
    void set_input(const dynamic_bitset<> &bits, BitsRange rng);
};

#endif // CIPHERANALYZER_H
