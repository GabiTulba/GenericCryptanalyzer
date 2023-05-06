#ifndef HELPERS_H
#define HELPERS_H

#include <boost/dynamic_bitset.hpp>

using namespace boost;
using namespace std;

struct BitsRange {
    size_t start;
    size_t len;
};

typedef pair<BitsRange, BitsRange> Connection;
typedef pair<string, Connection> NamedConnection;
typedef pair<dynamic_bitset<>, double> ProbEntry;
typedef vector<ProbEntry> ProbTableLine;
typedef vector<ProbTableLine> ProbTable;

ProbTable compute_diff_dist_table(const vector<size_t> &sbox);
bool increment_bits(dynamic_bitset<> &input);
dynamic_bitset<> to_dynamic_bitset(size_t input, size_t bit_size);

unsigned int to_uint(const dynamic_bitset<> &bitset);
dynamic_bitset<> from_uint(unsigned int x, int size);

size_t convert_to_index(const dynamic_bitset<> &bits);

#endif // HELPERS_H
