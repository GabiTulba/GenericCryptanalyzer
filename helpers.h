#ifndef HELPERS_H
#define HELPERS_H

#include <boost/dynamic_bitset.hpp>

using namespace boost;
using namespace std;

vector<vector<pair<dynamic_bitset<>, double>>>
compute_differential_table(const vector<size_t> &sbox);
bool increment_bits(dynamic_bitset<> &input);
dynamic_bitset<> to_dynamic_bitset(size_t input, size_t bit_size);

#endif // HELPERS_H
