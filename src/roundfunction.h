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
 * @brief a RoundFunction is a graph-like collection of boxes (or nodes) that are connected with eachother through
 * `Connection`s (or edges). The structure of a RoundFunction is a directed acyclic graph of boxes, with one source
 * `src` and one destination `dst`
 */
class RoundFunction {
  protected:
    /**
     * @brief entry point in the round function. Typically an IdentityBox
     */
    AbstractBoxPtr src;

    /**
     * @brief exit of the round function. Typically an IdentityBox
     */
    AbstractBoxPtr dst;

    /**
     * @brief a collection of all boxes that form this round. The boxes are stored in their topological order (i.e. If
     * `box1` sends bits to `box2` when `box1.notify_all()` is called, then `box1` comes before `box2` in the
     * topological sort).
     */
    vector<AbstractBoxPtr> boxes;

    /**
     * @brief the index in `boxes` of the current box in the search algorithm
     */
    size_t curr_box_idx;

    /**
     * @brief `vector` of all previous partial probabilities up to curr_box_idx. Used in calculating the final
     * probability of the round function.
     */
    vector<double> partial_prob;

    /**
     * @brief the minimum probability threshold such that the round function outputs a state.
     */
    double beta_thresh;

    /**
     * @brief a boolean value that should be true if and only if all possible outputs have been determined and returned
     */
    bool is_det;

    friend class CipherAnalyzer;

    /**
     * @brief advances to the next internal state state of the round function
     * @return false if there is no next state, otherwise returns true
     */
    bool advance_state();

    /**
     * @brief topological sort of boxes starting from `src`
     * @param src the current node in the depth-first search of the box-graph
     * @param top_sort topologically sorted `vector` of boxes, it is built during the dfs
     * @param is_visited a map that keeps track of which boxes have already been visited, used in dfs.
     */
    void top_sort_boxes(AbstractBoxPtr src, vector<AbstractBoxPtr> &top_sort, map<AbstractBoxPtr, bool> &is_visited);

    /**
     * @brief wrapper over `top_sort_boxes`
     * @param src is the root of the box-graph that will be visited, should be the same as `RoundFunction`'s `src`
     * @return a `vector` of boxes that are sorted topologically
     */
    vector<AbstractBoxPtr> sort_boxes(AbstractBoxPtr src);

  public:
    /**
     * @param src_id the name of the source box in the constructors map `constrs`
     * @param dst_idthe name of the destination box in the constructors map `constrs`
     * @param constrs a map of named AbstractBoxConstructor, used for enumerating the different types of boxes.
     * @param conns a map of connections from one named box to another, used for constructing the box-graph.
     */
    RoundFunction(string src_id, string dst_id, map<string, AbstractBoxConstructor> constrs,
                  map<string, vector<NamedConnection>> conns);

    /**
     * @brief getter for `is_det`
     * @return `is_det`
     */
    bool is_determined();

    /**
     * @brief returns the next possible output state that has a probability greater than `beta_thresh`
     * @return the next computed output state, otherwise `{dynamic_bitset<>(0), 0.0}`
     */
    ProbEntry get_next_state();

    /**
     * @brief sets a subrange `rng` of the input to the value of `bits`
     * @param bits the bits that will be put in the `in_bits` of `src`
     * @param rng the subrange in which bits will be put in `in_bits` of `src`
     */
    void set_input(const dynamic_bitset<> bits, BitsRange rng);

    /**
     * @brief setter for `beta_thresh`
     * @param the new value of `beta_thresh`
     */
    void set_threshold(double beta);
};

/**
 * @brief shorthand for std::shared_ptr<RoundFunction>
 */
typedef std::shared_ptr<RoundFunction> RoundFunctionPtr;

#endif // ROUNDFUNCTION_H
