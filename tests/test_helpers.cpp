#define BOOST_TEST_MODULE Test Helpers
#include <boost/test/included/unit_test.hpp>
#include <src/helpers/helpers.h>
#include <vector>

BOOST_AUTO_TEST_SUITE(test_popcnt)

BOOST_AUTO_TEST_CASE(bit_count_1) { BOOST_TEST(popcnt(0b11001010U) == 4); }

BOOST_AUTO_TEST_CASE(bit_count_2) { BOOST_TEST(popcnt(1U << 31) == 1); }

BOOST_AUTO_TEST_CASE(bit_count_3) { BOOST_TEST(popcnt(0b10101010101010101010101010101010U) == 16); }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_to_dynamic_bitset)

BOOST_AUTO_TEST_CASE(uint_1) {
    size_t bit_length = 8;
    unsigned int data = 0;
    vector<size_t> set_bits{7, 6, 4, 3, 2};

    dynamic_bitset<> correct(bit_length);
    for (auto bit_idx : set_bits) {
        correct.set(bit_idx);
        data |= (1 << bit_idx);
    }

    dynamic_bitset<> result = to_dynamic_bitset(data, bit_length);
    BOOST_TEST(result == correct);
}

BOOST_AUTO_TEST_CASE(uint_2) {
    size_t bit_length = 16;
    unsigned int data = 0;
    vector<size_t> set_bits{7, 5, 4, 2, 1};

    dynamic_bitset<> correct(bit_length);
    for (auto bit_idx : set_bits) {
        correct.set(bit_idx);
        data |= (1 << bit_idx);
    }

    dynamic_bitset<> result = to_dynamic_bitset(data, bit_length);
    BOOST_TEST(result == correct);
}

BOOST_AUTO_TEST_CASE(size_t_1) {
    size_t bit_length = 8;
    size_t data = 0;
    vector<size_t> set_bits{7, 6, 4, 3, 2};

    dynamic_bitset<> correct(bit_length);
    for (auto bit_idx : set_bits) {
        correct.set(bit_idx);
        data |= (1 << bit_idx);
    }

    dynamic_bitset<> result = to_dynamic_bitset(data, bit_length);
    BOOST_TEST(result == correct);
}

BOOST_AUTO_TEST_CASE(size_t_2) {
    size_t bit_length = 16;
    size_t data = 0;
    vector<size_t> set_bits{7, 5, 4, 2, 1};

    dynamic_bitset<> correct(bit_length);
    for (auto bit_idx : set_bits) {
        correct.set(bit_idx);
        data |= (1 << bit_idx);
    }

    dynamic_bitset<> result = to_dynamic_bitset(data, bit_length);
    BOOST_TEST(result == correct);
}

BOOST_AUTO_TEST_CASE(ull_1) {
    size_t bit_length = 32;
    unsigned long long data = 0;
    vector<size_t> set_bits{31, 28, 17, 12, 5, 3, 2};

    dynamic_bitset<> correct(bit_length);
    for (auto bit_idx : set_bits) {
        correct.set(bit_idx);
        data |= (1LL << bit_idx);
    }

    dynamic_bitset<> result = to_dynamic_bitset(data, bit_length);
    BOOST_TEST(result == correct);
}

BOOST_AUTO_TEST_CASE(ull_2) {
    size_t bit_length = 64;
    unsigned long long data = 0;
    vector<size_t> set_bits{63, 56, 44, 43, 42, 41, 31, 28, 17, 12, 5, 3, 2};

    dynamic_bitset<> correct(bit_length);
    for (auto bit_idx : set_bits) {
        correct.set(bit_idx);
        data |= (1LL << bit_idx);
    }

    dynamic_bitset<> result = to_dynamic_bitset(data, bit_length);
    BOOST_TEST(result == correct);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_dynamic_bitset_convert_to)

BOOST_AUTO_TEST_CASE(size_t_1) {
    size_t bit_length = 16;
    size_t result = 0;
    size_t correct = 0;
    vector<size_t> set_bits{7, 5, 4, 2, 1};

    dynamic_bitset<> bits(bit_length);
    for (auto bit_idx : set_bits) {
        bits.set(bit_idx);
        correct |= (1 << bit_idx);
    }

    result = convert_to_size_t(bits);
    BOOST_TEST(correct == result);
}

BOOST_AUTO_TEST_CASE(uint_1) {
    size_t bit_length = 16;
    unsigned int result = 0;
    unsigned int correct = 0;
    vector<size_t> set_bits{7, 5, 4, 2, 1};

    dynamic_bitset<> bits(bit_length);
    for (auto bit_idx : set_bits) {
        bits.set(bit_idx);
        correct |= (1 << bit_idx);
    }

    result = convert_to_size_t(bits);
    BOOST_TEST(correct == result);
}

BOOST_AUTO_TEST_CASE(ull_1) {
    size_t bit_length = 64;
    unsigned int result = 0;
    unsigned int correct = 0;
    vector<size_t> set_bits{63, 56, 44, 43, 42, 41, 31, 28, 17, 12, 5, 3, 2};

    dynamic_bitset<> bits(bit_length);
    for (auto bit_idx : set_bits) {
        bits.set(bit_idx);
        correct |= (1LL << bit_idx);
    }

    result = convert_to_ull(bits);
    BOOST_TEST(correct == result);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_increment_bits)

BOOST_AUTO_TEST_CASE(increment_once) {
    size_t bit_length = 16;
    dynamic_bitset<> bits(bit_length);
    dynamic_bitset<> correct(bit_length);
    bits.set(0);
    bits.set(1);
    bits.set(3);

    correct.set(0);
    correct.set(2);
    correct.set(3);

    increment_bits(bits);

    BOOST_TEST(bits == correct);
}

BOOST_AUTO_TEST_CASE(increment_twice) {
    size_t bit_length = 16;
    dynamic_bitset<> bits(bit_length);
    dynamic_bitset<> correct(bit_length);
    bits.set(0);
    bits.set(1);
    bits.set(3);

    correct.set(1);
    correct.set(2);
    correct.set(3);

    increment_bits(bits);
    increment_bits(bits);

    BOOST_TEST(bits == correct);
}

BOOST_AUTO_TEST_CASE(consistent_bit_count_1) {
    size_t bit_length = 16;
    dynamic_bitset<> it(bit_length);
    it.set(0);

    while (increment_bits(it)) {
        BOOST_TEST(it.count() == 1);
    }
}

BOOST_AUTO_TEST_CASE(consistent_bit_count_2) {
    size_t bit_length = 16;
    dynamic_bitset<> it(bit_length);
    it.set(0, 3, 1);

    while (increment_bits(it)) {
        BOOST_TEST(it.count() == 3);
    }
}

BOOST_AUTO_TEST_CASE(one_bit_total_iterations) {
    size_t bit_length = 16;
    dynamic_bitset<> it(bit_length);
    it.set(0);

    int states = 1;
    while (increment_bits(it)) {
        states++;
    }

    BOOST_TEST(states == bit_length);
}

BOOST_AUTO_TEST_CASE(zero_bits_total_iterations) {
    size_t bit_length = 16;
    dynamic_bitset<> it(bit_length);

    int states = 1;
    while (increment_bits(it)) {
        states++;
    }

    BOOST_TEST(states == 1);
}

BOOST_AUTO_TEST_CASE(two_bits_total_iterations) {
    size_t bit_length = 16;
    dynamic_bitset<> it(bit_length);
    it.set(0);
    it.set(1);

    int states = 1;
    while (increment_bits(it)) {
        states++;
    }

    BOOST_TEST(states == bit_length * (bit_length - 1) / 2);
}

BOOST_AUTO_TEST_CASE(all_states_total_iterations) {
    size_t bit_length = 16;
    dynamic_bitset<> it(bit_length);

    int states = 1;
    for (int i = 0; i < bit_length; i++) {
        it.set(0, i, 1);
        states++;

        while (increment_bits(it)) {
            states++;
        }
    }

    BOOST_TEST(states == (1 << bit_length));
}

BOOST_AUTO_TEST_SUITE_END()

const vector<size_t> sbox{0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7};
const vector<size_t> bad_sbox_1{0x1, 0x3, 0x2};
const vector<size_t> bad_sbox_2{0x1, 0x0, 0x2, 0x4};

BOOST_AUTO_TEST_SUITE(test_compute_diff_dist_table)

BOOST_AUTO_TEST_CASE(test_throw_on_bad_sbox) {
    BOOST_CHECK_THROW(compute_diff_dist_table(bad_sbox_1), std::logic_error);
    BOOST_CHECK_THROW(compute_diff_dist_table(bad_sbox_2), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_input_size) {
    size_t input_size = sbox.size();
    ProbTable result = compute_diff_dist_table(sbox);
    BOOST_TEST(result.size() == input_size);
}

BOOST_AUTO_TEST_CASE(test_output_max_size) {
    size_t output_max_size = *max_element(sbox.begin(), sbox.end()) + 1;
    ProbTable result = compute_diff_dist_table(sbox);

    for (auto &line : result) {
        BOOST_TEST(line.size() <= output_max_size);
    }
}

BOOST_AUTO_TEST_CASE(line_probability_sum_1, *unit_test::tolerance(1e-8)) {
    ProbTable result = compute_diff_dist_table(sbox);

    for (auto &line : result) {
        double sum = 0.0;
        for (auto &elem : line) {
            sum += elem.second;
        }
        BOOST_TEST(sum == 1);
    }
}

BOOST_AUTO_TEST_CASE(test_non_impossible_differentials) {
    ProbTable result = compute_diff_dist_table(sbox);

    for (auto &line : result) {
        for (auto &elem : line) {
            BOOST_TEST(elem.second > 0);
        }
    }
}

BOOST_AUTO_TEST_CASE(sorted_elems_by_probability) {
    ProbTable result = compute_diff_dist_table(sbox);

    for (auto &line : result) {
        double prev_prob = 1.0;
        for (auto &elem : line) {
            BOOST_TEST(elem.second <= prev_prob);
            prev_prob = elem.second;
        }
    }
}

BOOST_AUTO_TEST_CASE(correct_output_bit_length, *unit_test::tolerance(1e-8)) {
    ProbTable result = compute_diff_dist_table(sbox);
    size_t bit_length = popcnt(*max_element(sbox.begin(), sbox.end()));

    for (auto &line : result) {
        for (auto &elem : line) {
            BOOST_TEST(elem.first.size() == bit_length);
        }
    }
}

BOOST_AUTO_TEST_CASE(input_differences, *unit_test::tolerance(1e-8)) {
    ProbTable result = compute_diff_dist_table(sbox);
    size_t bit_length = popcnt(*max_element(sbox.begin(), sbox.end()));
    ProbTableLine correct{{to_dynamic_bitset(8U, bit_length), 6. / 16},
                          {to_dynamic_bitset(3U, bit_length), 4. / 16},
                          {to_dynamic_bitset(2U, bit_length), 2. / 16},
                          {to_dynamic_bitset(4U, bit_length), 2. / 16},
                          {to_dynamic_bitset(14U, bit_length), 2. / 16}};
    BOOST_TEST(result[0xe] == correct);
}

BOOST_AUTO_TEST_SUITE_END()
