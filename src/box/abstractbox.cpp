#include "abstractbox.h"

void AbstractBox::notify_all() {
    for (auto &[dst_box, conn] : dst_boxes) {
        dynamic_bitset<> out_bits_subrng = out_bits >> conn.first.start;
        out_bits_subrng.resize(conn.first.len);
        dst_box->set_input(out_bits_subrng, conn.second);
    }
}

void AbstractBox::reset_determination() {
    is_det = false;
    prob = 0.0;
    out_bits.set(0, out_bits.size(), false);
}

AbstractBox::AbstractBox(size_t in_size, size_t out_size)
    : in_bits(dynamic_bitset<>(in_size, 0)), out_bits(dynamic_bitset<>(out_size, 0)),
      dst_boxes(vector<pair<AbstractBoxPtr, Connection>>()), src_boxes(vector<pair<AbstractBoxPtr, Connection>>()),
      is_det(false), prob(0.0) {}

void AbstractBox::add_dest(AbstractBoxPtr dst_box, BitsRange out_rng, BitsRange in_rng) noexcept(false) {
    if (dst_box == nullptr) {
        throw std::logic_error("destination boxes pointers must be non-null");
    }
    if (out_rng.start + out_rng.len > out_bits.size()) {
        throw std::logic_error("output BitsRange is out of output bits range");
    }
    if (out_rng.len != in_rng.len) {
        throw std::logic_error("input and output bit range lengths aren't equal");
    }
    if (in_rng.start + in_rng.len > dst_box->input_size()) {
        throw std::logic_error("input BitsRange is out of input bits range");
    }
    dst_boxes.push_back(make_pair(dst_box, make_pair(out_rng, in_rng)));
}

void AbstractBox::add_src(AbstractBoxPtr src_box, BitsRange out_rng, BitsRange in_rng) noexcept(false) {
    if (src_box == nullptr) {
        throw std::logic_error("destination boxes pointers must be non-null");
    }
    if (out_rng.start + out_rng.len > src_box->out_bits.size()) {
        throw std::logic_error("output BitsRange is out of output bits range");
    }
    if (out_rng.len != in_rng.len) {
        throw std::logic_error("input and output bit range lengths aren't equal");
    }
    if (in_rng.start + in_rng.len > input_size()) {
        throw std::logic_error("input BitsRange is out of input bits range");
    }
    src_boxes.push_back(make_pair(src_box, make_pair(out_rng, in_rng)));
}

const dynamic_bitset<> &AbstractBox::get_input() { return in_bits; }

const dynamic_bitset<> &AbstractBox::get_output() { return out_bits; }

size_t AbstractBox::input_size() { return in_bits.size(); }

size_t AbstractBox::output_size() { return out_bits.size(); }

bool AbstractBox::is_determined() { return is_det; }

void AbstractBox::set_input(dynamic_bitset<> bits, const BitsRange &range) {
    reset_determination();
    this->in_bits.set(range.start, range.len, false);
    bits.resize(in_bits.size());
    bits <<= range.start;
    this->in_bits |= bits;
}

double AbstractBox::get_probability() { return prob; }
