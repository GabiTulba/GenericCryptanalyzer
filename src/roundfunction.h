#ifndef ROUNDFUNCTION_H
#define ROUNDFUNCTION_H

#include "box/abstractbox.h"
#include "box/ebox.h"
#include "box/identitybox.h"
#include "box/pbox.h"
#include "box/sbox.h"
#include "helpers/helpers.h"

class RoundFunction {
  protected:
    AbstractBoxPtr src;
    AbstractBoxPtr dst;
    size_t curr_box_idx;

    // this should be in the topological sort order of the box-graph
    vector<AbstractBoxPtr> boxes;
    vector<double> partial_prob;
    double beta_thresh;
    bool is_det;

    friend class CipherAnalyzer;

    // advances to the next internal state state
    // returns false if there is no next state
    bool advance_state();

    // topological sort of boxes starting from the source AbstractBox
    void top_sort_boxes(AbstractBoxPtr src, vector<AbstractBoxPtr> &top_sort, map<AbstractBoxPtr, bool> &is_visited);

    // wrapper over the previous function
    vector<AbstractBoxPtr> sort_boxes(AbstractBoxPtr src);

  public:
    RoundFunction(string src_id, string dst_id, map<string, AbstractBoxConstructor> constrs,
                  map<string, vector<NamedConnection>> conns);

    bool is_determined();

    ProbEntry get_next_entry();

    void set_input(const dynamic_bitset<> bits, BitsRange rng);
    void set_threshold(double beta);
};

typedef std::shared_ptr<RoundFunction> RoundFunctionPtr;

#endif // ROUNDFUNCTION_H
