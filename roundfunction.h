#ifndef ROUNDFUNCTION_H
#define ROUNDFUNCTION_H

#include "abstractbox.h"
#include "ebox.h"
#include "identitybox.h"
#include "pbox.h"
#include "sbox.h"

class RoundFunction {
  protected:
    std::shared_ptr<AbstractBox> src;
    std::shared_ptr<AbstractBox> dst;
    size_t curr_state_box_idx;

    // this should be in the topological sort order of the box-graph
    // TODO: construct the topological sort so the constructors can be given in
    // arbitrary order
    vector<std::shared_ptr<AbstractBox>> boxes;
    vector<double> partial_probabilities;
    double beta_threshold;
    bool determined;

    friend class CipherAnalyzer;

    // advances to the next internal state state
    // returns false if there is no next state
    bool advance_state();

    // topological sort of boxes starting from the source AbstractBox
    void top_sort_boxes(std::shared_ptr<AbstractBox> src,
                        vector<std::shared_ptr<AbstractBox>> &sort,
                        map<std::shared_ptr<AbstractBox>, bool> &visited);

    // wrapper over the previous function
    vector<std::shared_ptr<AbstractBox>>
    sort_boxes(std::shared_ptr<AbstractBox> src);

  public:
    RoundFunction(
        string src_id, string dst_id,
        map<string, function<std::shared_ptr<AbstractBox>()>> constructors,
        map<string, vector<pair<string, pair<bits_range, bits_range>>>>
            connections);

    bool is_determined();

    pair<dynamic_bitset<>, double> get_next_differential();

    void set_input(const dynamic_bitset<> bits, bits_range range);
    void set_threshold(double beta);
};

#endif // ROUNDFUNCTION_H
