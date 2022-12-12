#include "abstractbox.h"

void AbstractBox::notify_all() {
    for (auto &dest_box : dest_boxes) {
        auto output_bits_subrange = output_bits >> dest_box.second.first.start;
        output_bits_subrange.resize(dest_box.second.first.len);
        dest_box.first->set_input(output_bits_subrange, dest_box.second.second);
    }
}

void AbstractBox::reset_determination() {
    determined = false;
    probability = 0.0;
}

AbstractBox::AbstractBox(
    size_t input_size, size_t output_size,
    const vector<pair<std::shared_ptr<AbstractBox>,
                      pair<bits_range, bits_range>>> &dest_boxes)
    : input_bits(dynamic_bitset<>(input_size, 0)),
      output_bits(dynamic_bitset<>(output_size, 0)), dest_boxes(dest_boxes),
      determined(false), probability(0.0) {
    for (auto &dest_box : dest_boxes) {
        assert(dest_box.first != nullptr);
        assert(dest_box.second.first.start + dest_box.second.first.len <=
               output_bits.size());
        assert(dest_box.second.first.len == dest_box.second.second.len);
        assert(dest_box.second.second.start + dest_box.second.second.len <=
               dest_box.first->input_size());
    }
}

AbstractBox::AbstractBox(size_t input_size, size_t output_size)
    : input_bits(dynamic_bitset<>(input_size, 0)),
      output_bits(dynamic_bitset<>(output_size, 0)),
      dest_boxes(vector<pair<std::shared_ptr<AbstractBox>,
                             pair<bits_range, bits_range>>>()),
      determined(false), probability(0.0) {}

void AbstractBox::add_dest(std::shared_ptr<AbstractBox> dest_box,
                           bits_range output_range, bits_range input_range) {
    assert(dest_box != nullptr);
    assert(output_range.start + output_range.len <= output_bits.size());
    assert(input_range.start + input_range.len <= dest_box->input_bits.size());
    dest_boxes.push_back(
        make_pair(dest_box, make_pair(output_range, input_range)));
}

const dynamic_bitset<> &AbstractBox::get_input() { return input_bits; }

const dynamic_bitset<> &AbstractBox::get_output() { return output_bits; }

size_t AbstractBox::input_size() { return input_bits.size(); }

size_t AbstractBox::output_size() { return output_bits.size(); }

bool AbstractBox::is_determined() { return determined; }

void AbstractBox::set_input(dynamic_bitset<> bits, const bits_range &range) {
    this->input_bits.set(range.start, range.len, false);
    bits.resize(input_bits.size());
    bits <<= range.start;
    this->input_bits |= bits;
}

double AbstractBox::get_probability() { return probability; }
