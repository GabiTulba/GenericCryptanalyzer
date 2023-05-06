/** @file roundfunction.h
 *  @brief implementation of the RoundFunction class
 */
#ifndef ROUNDFUNCTION_H
#define ROUNDFUNCTION_H

#include "box/abstractbox.h"
#include "box/ebox.h"
#include "box/identitybox.h"
#include "box/pbox.h"
#include "box/sbox.h"
#include "box/xorbox.h"

/**
 * @brief The RoundFunction class
 */
class RoundFunction {
  protected:
    /**
     * @brief src
     */
    AbstractBoxPtr src;

    /**
     * @brief dst
     */
    AbstractBoxPtr dst;

    /**
     * @brief curr_box_idx
     */
    size_t curr_box_idx;

    // this should be in the topological sort order of the box-graph
    /**
     * @brief boxes
     */
    vector<AbstractBoxPtr> boxes;

    /**
     * @brief partial_prob
     */
    vector<double> partial_prob;

    /**
     * @brief beta_thresh
     */
    double beta_thresh;

    /**
     * @brief is_det
     */
    bool is_det;

    friend class CipherAnalyzer;

    // advances to the next internal state state
    // returns false if there is no next state
    /**
     * @brief advance_state
     * @return
     */
    bool advance_state();

    // topological sort of boxes starting from the source AbstractBox
    /**
     * @brief top_sort_boxes
     * @param src
     * @param top_sort
     * @param is_visited
     */
    void top_sort_boxes(AbstractBoxPtr src, vector<AbstractBoxPtr> &top_sort, map<AbstractBoxPtr, bool> &is_visited);

    // wrapper over the previous function
    /**
     * @brief sort_boxes
     * @param src
     * @return
     */
    vector<AbstractBoxPtr> sort_boxes(AbstractBoxPtr src);

  public:
    /**
     * @brief RoundFunction
     * @param src_id
     * @param dst_id
     * @param constrs
     * @param conns
     */
    RoundFunction(string src_id, string dst_id, map<string, AbstractBoxConstructor> constrs,
                  map<string, vector<NamedConnection>> conns);

    /**
     * @brief is_determined
     * @return
     */
    bool is_determined();

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
    void set_input(const dynamic_bitset<> bits, BitsRange rng);

    /**
     * @brief set_threshold
     * @param beta
     */
    void set_threshold(double beta);
};

/**
 * @brief shorthand for std::shared_ptr<RoundFunction>
 */
typedef std::shared_ptr<RoundFunction> RoundFunctionPtr;

#endif // ROUNDFUNCTION_H
