#ifndef CIPHERANALYZER_H
#define CIPHERANALYZER_H

#include "helpers.h"
#include "roundfunction.h"

class CipherAnalyzer {
  protected:
    double global_threshold;
    vector<double> optimal_probabilities;
    vector<double> round_probabilities;
    vector<std::shared_ptr<RoundFunction>> rounds;
    size_t curr_round_idx;

    bool advance_state();

  public:
    CipherAnalyzer(vector<std::shared_ptr<RoundFunction>> rounds,
                   double global_threshold,
                   vector<double> optimal_probabilities);

    CipherAnalyzer(vector<std::shared_ptr<RoundFunction>> rounds,
                   double global_threshold);

    pair<dynamic_bitset<>, double> get_next_differential();
    void set_input(const dynamic_bitset<> &bits, bits_range range);
};

#endif // CIPHERANALYZER_H
