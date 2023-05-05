#ifndef ABSTRACTBOX_H
#define ABSTRACTBOX_H

#include "helpers/helpers.h"
#include <boost/dynamic_bitset.hpp>
#include <vector>

using namespace std;
using namespace boost;

class AbstractBox;

typedef std::shared_ptr<AbstractBox> AbstractBoxPtr;
typedef function<AbstractBoxPtr()> AbstractBoxConstructor;

class AbstractBox {
  protected:
    dynamic_bitset<> in_bits;
    dynamic_bitset<> out_bits;
    vector<pair<AbstractBoxPtr, Connection>> dst_boxes;
    bool is_det;
    double prob;

    friend class RoundFunction;

  public:
    AbstractBox(size_t in_size, size_t out_size,
                const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes);

    AbstractBox(size_t in_size, size_t out_size);

    void add_dest(AbstractBoxPtr dst_box, BitsRange out_range,
                  BitsRange in_range);

    // getters
    const dynamic_bitset<> &get_input();
    const dynamic_bitset<> &get_output();
    size_t input_size();
    size_t output_size();
    bool is_determined();

    // setters
    virtual void set_input(dynamic_bitset<> bits, const BitsRange &rng);

    // method to notify all the destination boxes after the
    // output of the box is determined
    void notify_all();

    // method to determine the next best output sorted by probabilities
    // returns true if there is at most one more possible determination state
    virtual void determine_next() = 0;

    // set the process to be undetermined
    virtual void reset_determination();

    // method to obtain the probability of the current differential
    // characteristic
    double get_probability();
};

#endif // ABSTRACTBOX_H
