#ifndef CIPHERANALYZER_H
#define CIPHERANALYZER_H

#include "roundfunction.h"

class CipherAnalyzer {
  protected:
    double global_threshold;
    vector<double> optimal_probabilities;
    vector<double> round_probabilities;
    vector<std::shared_ptr<RoundFunction>> rounds;
    size_t curr_round_idx;

    bool advance_state();
    bool increment_bits(dynamic_bitset<> &input);

  public:
    CipherAnalyzer(
        size_t round_count, double global_threshold, string src_id,
        string dst_id, vector<double> optimal_probabilities,
        map<string, function<std::shared_ptr<AbstractBox>()>> constructors,
        map<string, vector<pair<string, pair<bits_range, bits_range>>>>
            connections);

    CipherAnalyzer(
        size_t round_count, double global_threshold, string src_id,
        string dst_id,
        map<string, function<std::shared_ptr<AbstractBox>()>> constructors,
        map<string, vector<pair<string, pair<bits_range, bits_range>>>>
            connections);

    pair<dynamic_bitset<>, double> get_next_differential();
    void set_input(const dynamic_bitset<> &bits, bits_range range);
};

#endif // CIPHERANALYZER_H
