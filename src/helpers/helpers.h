#ifndef HELPERS_H
#define HELPERS_H

#include <boost/dynamic_bitset.hpp>

using namespace boost;
using namespace std;

/**
 * @brief BitsRange represents a way to represent a subrange of bits of a box
 */
struct BitsRange {
    size_t start;
    size_t len;
};

/**
 * @brief shorthand for pair<BitsRange, BitsRange>. Represents a connection between two boxes, the first BitsRange is
 * the subrange of the first box, while the second BitsRange is the subrange of the second box
 */
typedef pair<BitsRange, BitsRange> Connection;

/**
 * @brief shorthand for pair<string, Connection>
 */
typedef pair<string, Connection> NamedConnection;

/**
 * @brief shorthand for pair<dynamic_bitset<>, double>. Represents a pair between an output and it's probability of an
 * sbox given it's input
 */
typedef pair<dynamic_bitset<>, double> ProbEntry;

/**
 * @brief shorthand for vector<ProbEntry>. Represents a whole line of a probability table
 */
typedef vector<ProbEntry> ProbTableLine;

/**
 * @brief shorthand for vector<ProbTableLine>. A probability table is a compact way to describe the behaviour of an sbox
 * when analyzing the difference of two inputs.
 */
typedef vector<ProbTableLine> ProbTable;

/**
 * @brief computes the difference distribution table of an sbox. Used for differential cryptanalysis
 * @param sbox a substitution function
 * @return the difference distribution table (DDT) of the sbox
 *
 * @pre input_size (i.e. sbox.size()) and output_size (i.e. max(sbox) + 1) should be powers of two
 */
ProbTable compute_diff_dist_table(const vector<size_t> &sbox);

/**
 * @brief given some input, calculates the next lexicografically smallest binary array with the same number of bits. It
 * sorts all entries for an input by their probability
 * @param input a binary array
 * @return `true` if there was at least one next
 */
bool increment_bits(dynamic_bitset<> &input);

/**
 * @brief converts a `size_t` to a `dynamic_bitset` of size `bit_size`
 * @param input a `size_t`
 * @param bit_size the size in bits
 * @return a `dynamic_bitset<>` representing the binary array computed from input
 */
dynamic_bitset<> to_dynamic_bitset(size_t input, size_t bit_size);

/**
 * @brief converts an `unsigned int` to a `dynamic_bitset` of size `bit_size`
 * @param input a `size_t`
 * @param bit_size the size in bits
 * @return a `dynamic_bitset<>` representing the binary array computed from input
 */
dynamic_bitset<> to_dynamic_bitset(unsigned int input, size_t bit_size);

/**
 * @brief converts a `dynamic_bitset` to a `size_t`
 * @param bits a `dynamic_bitset` representing a binary array
 * @return a `size_t` representing the number formed from the binray array given as input
 */
size_t convert_to_index(const dynamic_bitset<> &bits);

/**
 * @brief converts a `dynamic_bitset` to an `unsigned int`
 * @param bits a `dynamic_bitset` representing a binary array
 * @return an `unsigned int` representing the number formed from the binray array given as input
 */
unsigned int convert_to_uint(const dynamic_bitset<> &bits);

#endif // HELPERS_H
