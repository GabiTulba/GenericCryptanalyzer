#include "abstractbox.h"

void AbstractBox::notify_all() {
    for (auto &dst_box : dst_boxes) {
        auto out_bits_subrng = out_bits >> dst_box.second.first.start;
        out_bits_subrng.resize(dst_box.second.first.len);
        dst_box.first->set_input(out_bits_subrng, dst_box.second.second);
    }
}

void AbstractBox::reset_determination() {
    is_det = false;
    prob = 0.0;
}

AbstractBox::AbstractBox(
    size_t in_size, size_t out_size,
    const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes)
    : in_bits(dynamic_bitset<>(in_size, 0)),
      out_bits(dynamic_bitset<>(out_size, 0)), dst_boxes(dst_boxes),
      is_det(false), prob(0.0) {
    for (auto &dst_box : dst_boxes) {
        assert(dst_box.first != nullptr);
        assert(dst_box.second.first.start + dst_box.second.first.len <=
               out_bits.size());
        assert(dst_box.second.first.len == dst_box.second.second.len);
        assert(dst_box.second.second.start + dst_box.second.second.len <=
               dst_box.first->input_size());
    }
}

AbstractBox::AbstractBox(size_t in_size, size_t out_size)
    : in_bits(dynamic_bitset<>(in_size, 0)),
      out_bits(dynamic_bitset<>(out_size, 0)),
      dst_boxes(vector<pair<AbstractBoxPtr, Connection>>()), is_det(false),
      prob(0.0) {}

void AbstractBox::add_dest(AbstractBoxPtr dst_box, BitsRange out_rng,
                           BitsRange in_rng) {
    assert(dst_box != nullptr);
    assert(out_rng.start + out_rng.len <= out_bits.size());
    assert(in_rng.start + in_rng.len <= dst_box->in_bits.size());
    dst_boxes.push_back(make_pair(dst_box, make_pair(out_rng, in_rng)));
}

const dynamic_bitset<> &AbstractBox::get_input() { return in_bits; }

const dynamic_bitset<> &AbstractBox::get_output() { return out_bits; }

size_t AbstractBox::input_size() { return in_bits.size(); }

size_t AbstractBox::output_size() { return out_bits.size(); }

bool AbstractBox::is_determined() { return is_det; }

void AbstractBox::set_input(dynamic_bitset<> bits, const BitsRange &range) {
    this->in_bits.set(range.start, range.len, false);
    bits.resize(in_bits.size());
    bits <<= range.start;
    this->in_bits |= bits;
}

double AbstractBox::get_probability() { return prob; }
