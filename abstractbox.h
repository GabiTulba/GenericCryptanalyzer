#ifndef ABSTRACTBOX_H
#define ABSTRACTBOX_H

#include <boost/dynamic_bitset.hpp>
#include <vector>

using namespace std;
using namespace boost;

struct bits_range {
    size_t start;
    size_t len;
};

class AbstractBox {
  protected:
    dynamic_bitset<> input_bits;
    dynamic_bitset<> output_bits;
    vector<pair<std::shared_ptr<AbstractBox>, pair<bits_range, bits_range>>>
        dest_boxes;
    bool determined;
    double probability;

    friend class RoundFunction;

  public:
    AbstractBox(size_t input_size, size_t output_size,
                const vector<pair<std::shared_ptr<AbstractBox>,
                                  pair<bits_range, bits_range>>> &dest_boxes);

    AbstractBox(size_t input_size, size_t output_size);

    void add_dest(std::shared_ptr<AbstractBox> dest_box,
                  bits_range output_range, bits_range input_range);

    // getters
    const dynamic_bitset<> &get_input();
    const dynamic_bitset<> &get_output();
    size_t input_size();
    size_t output_size();
    bool is_determined();

    // setters
    virtual void set_input(dynamic_bitset<> bits, const bits_range &range);

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
