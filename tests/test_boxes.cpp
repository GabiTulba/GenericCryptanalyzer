#define BOOST_TEST_MODULE Test Helpers
#include <boost/test/included/unit_test.hpp>
#include <memory>
#include <set>
#include <src/box/abstractbitshiftbox.h>
#include <src/box/abstractbox.h>
#include <src/box/ebox.h>
#include <src/box/identitybox.h>
#include <src/box/pbox.h>
#include <src/box/sbox.h>
#include <src/box/xorbox.h>
#include <src/helpers/helpers.h>

const vector<size_t> s_box{0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7};

BOOST_AUTO_TEST_SUITE(test_ebox)

const vector<size_t> bit_expansion{0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3};
const vector<size_t> bad_bit_expansion{0, 1, 2, 3, 7};

BOOST_AUTO_TEST_CASE(bad_constuctor) {
    size_t bit_length_in = *max_element(bit_expansion.begin(), bit_expansion.end()) + 1;
    size_t bit_length_out = bit_expansion.size();

    BOOST_CHECK_THROW(EBox(bit_length_in - 1, bit_length_out, bad_bit_expansion), std::logic_error);
    BOOST_CHECK_THROW(EBox(bit_length_in, bit_length_out - 1, bad_bit_expansion), std::logic_error);
    BOOST_CHECK_THROW(EBox(bit_length_in, bit_length_out, bad_bit_expansion), std::logic_error);
}

BOOST_AUTO_TEST_CASE(constuctor) {
    size_t bit_length_in = *max_element(bit_expansion.begin(), bit_expansion.end()) + 1;
    size_t bit_length_out = bit_expansion.size();
    EBox ebox(bit_length_in, bit_length_out, bit_expansion);

    BOOST_TEST(ebox.get_input().size() == bit_length_in);
    BOOST_TEST(ebox.get_output().size() == bit_length_out);
    BOOST_TEST(ebox.is_determined() == false);
    BOOST_TEST(ebox.get_probability() == 0.0);
    BOOST_TEST(ebox.get_input() == to_dynamic_bitset(0U, bit_length_in));
    BOOST_TEST(ebox.get_output() == to_dynamic_bitset(0U, bit_length_out));
}

BOOST_AUTO_TEST_CASE(input_get_set_simple) {
    size_t bit_length_in = *max_element(bit_expansion.begin(), bit_expansion.end()) + 1;
    size_t bit_length_out = bit_expansion.size();
    EBox ebox(bit_length_in, bit_length_out, bit_expansion);

    ebox.set_input(to_dynamic_bitset(123U, bit_length_in), BitsRange(0, bit_length_in));

    BOOST_TEST(ebox.get_input() == to_dynamic_bitset(123U, bit_length_in));
}

BOOST_AUTO_TEST_CASE(input_get_set_complex) {
    size_t bit_length_in = *max_element(bit_expansion.begin(), bit_expansion.end()) + 1;
    size_t bit_length_out = bit_expansion.size();
    EBox ebox(bit_length_in, bit_length_out, bit_expansion);

    BOOST_REQUIRE(bit_length_in % 2 == 0);

    unsigned int first_half = 11U;
    unsigned int second_half = 14U;
    ebox.set_input(to_dynamic_bitset(first_half, bit_length_in / 2), BitsRange(0, bit_length_in / 2));
    ebox.set_input(to_dynamic_bitset(second_half, bit_length_in / 2), BitsRange(bit_length_in / 2, bit_length_in / 2));

    BOOST_TEST(ebox.get_input() == to_dynamic_bitset(first_half + (second_half << (bit_length_in / 2)), bit_length_in));
}

BOOST_AUTO_TEST_CASE(determine_next_idempotent) {
    size_t bit_length_in = *max_element(bit_expansion.begin(), bit_expansion.end()) + 1;
    size_t bit_length_out = bit_expansion.size();
    EBox ebox(bit_length_in, bit_length_out, bit_expansion);

    ebox.set_input(to_dynamic_bitset(14U, bit_length_in), BitsRange(0, bit_length_in));
    ebox.determine_next();

    BOOST_TEST(ebox.is_determined());
    BOOST_TEST(ebox.get_probability() == 1.0);
    BOOST_TEST(ebox.get_output() == to_dynamic_bitset(3598U, bit_length_out));

    ebox.determine_next();

    BOOST_TEST(ebox.is_determined());
    BOOST_TEST(ebox.get_probability() == 1.0);
    BOOST_TEST(ebox.get_output() == to_dynamic_bitset(3598U, bit_length_out));
}

BOOST_AUTO_TEST_CASE(set_input_after_determine_next) {
    size_t bit_length_in = *max_element(bit_expansion.begin(), bit_expansion.end()) + 1;
    size_t bit_length_out = bit_expansion.size();
    EBox ebox(bit_length_in, bit_length_out, bit_expansion);

    unsigned int data_in_1 = 14;
    unsigned int data_in_2 = 29;
    ebox.set_input(to_dynamic_bitset(data_in_1, bit_length_in), BitsRange(0, bit_length_in));
    ebox.determine_next();
    ebox.set_input(to_dynamic_bitset(data_in_2, bit_length_in), BitsRange(0, bit_length_in));

    BOOST_TEST(ebox.is_determined() == false);
    BOOST_TEST(ebox.get_probability() == 0.0);
    BOOST_TEST(ebox.get_input() == to_dynamic_bitset(data_in_2, bit_length_in));
    BOOST_TEST(ebox.get_output() == to_dynamic_bitset(0U, bit_length_out));
}

BOOST_AUTO_TEST_CASE(determine_next_after_set_input_reset) {
    size_t bit_length_in = *max_element(bit_expansion.begin(), bit_expansion.end()) + 1;
    size_t bit_length_out = bit_expansion.size();
    EBox ebox(bit_length_in, bit_length_out, bit_expansion);

    unsigned int data_in_1 = 14;
    unsigned int data_in_2 = 29;
    unsigned int data_out = 3357;
    ebox.set_input(to_dynamic_bitset(data_in_1, bit_length_in), BitsRange(0, bit_length_in));
    ebox.determine_next();
    ebox.set_input(to_dynamic_bitset(data_in_2, bit_length_in), BitsRange(0, bit_length_in));
    ebox.determine_next();

    BOOST_TEST(ebox.is_determined());
    BOOST_TEST(ebox.get_probability());
    BOOST_TEST(ebox.get_output() == to_dynamic_bitset(data_out, bit_length_out));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_pbox)

const vector<size_t> bit_perm{0, 1, 2, 3, 7, 6, 5, 4, 8, 9, 11, 10};
const vector<size_t> bad_bit_perm_1{0, 1, 3};
const vector<size_t> bad_bit_perm_2{2, 1, 3};
const vector<size_t> bad_bit_perm_3{1, 1, 2};

BOOST_AUTO_TEST_CASE(bad_constuctor) {
    BOOST_CHECK_THROW(PBox(bad_bit_perm_1.size(), bad_bit_perm_1), std::logic_error);
    BOOST_CHECK_THROW(PBox(bad_bit_perm_2.size(), bad_bit_perm_2), std::logic_error);
    BOOST_CHECK_THROW(PBox(bad_bit_perm_3.size(), bad_bit_perm_3), std::logic_error);
}

BOOST_AUTO_TEST_CASE(constructor) {
    size_t bit_length = bit_perm.size();
    PBox pbox(bit_length, bit_perm);

    BOOST_TEST(pbox.get_input().size() == bit_length);
    BOOST_TEST(pbox.get_output().size() == bit_length);
    BOOST_TEST(pbox.is_determined() == false);
    BOOST_TEST(pbox.get_probability() == 0.0);
    BOOST_TEST(pbox.get_input() == to_dynamic_bitset(0U, bit_length));
    BOOST_TEST(pbox.get_output() == to_dynamic_bitset(0U, bit_length));
}

BOOST_AUTO_TEST_CASE(input_get_set_simple) {
    size_t bit_length = bit_perm.size();
    unsigned int data_in = 2359;
    PBox pbox(bit_length, bit_perm);

    pbox.set_input(to_dynamic_bitset(data_in, bit_length), BitsRange(0, bit_length));

    BOOST_TEST(pbox.get_input() == to_dynamic_bitset(data_in, bit_length));
}

BOOST_AUTO_TEST_CASE(input_get_set_complex) {
    size_t bit_length = bit_perm.size();
    PBox pbox(bit_length, bit_perm);

    BOOST_REQUIRE(bit_length % 2 == 0);

    unsigned int first_half = 13;
    unsigned int second_half = 24;
    pbox.set_input(to_dynamic_bitset(first_half, bit_length / 2), BitsRange(0, bit_length / 2));
    pbox.set_input(to_dynamic_bitset(second_half, bit_length / 2), BitsRange(bit_length / 2, bit_length / 2));

    BOOST_TEST(pbox.get_input() == to_dynamic_bitset(first_half + (second_half << (bit_length / 2)), bit_length));
}

BOOST_AUTO_TEST_CASE(determine_next_idempotent) {
    size_t bit_length = bit_perm.size();
    unsigned int data_in = 2359;
    unsigned data_out = 1479;
    PBox pbox(bit_length, bit_perm);

    pbox.set_input(to_dynamic_bitset(data_in, bit_length), BitsRange(0, bit_length));
    pbox.determine_next();

    BOOST_TEST(pbox.is_determined());
    BOOST_TEST(pbox.get_probability() == 1.0);
    BOOST_TEST(pbox.get_output() == to_dynamic_bitset(data_out, bit_length));

    pbox.determine_next();

    BOOST_TEST(pbox.is_determined());
    BOOST_TEST(pbox.get_probability() == 1.0);
    BOOST_TEST(pbox.get_output() == to_dynamic_bitset(data_out, bit_length));
}

BOOST_AUTO_TEST_CASE(set_input_after_determine_next) {
    size_t bit_length = bit_perm.size();
    PBox pbox(bit_length, bit_perm);

    unsigned int data_in_1 = 123;
    unsigned int data_in_2 = 357;
    pbox.set_input(to_dynamic_bitset(data_in_1, bit_length), BitsRange(0, bit_length));
    pbox.determine_next();
    pbox.set_input(to_dynamic_bitset(data_in_2, bit_length), BitsRange(0, bit_length));

    BOOST_TEST(pbox.is_determined() == false);
    BOOST_TEST(pbox.get_probability() == 0.0);
    BOOST_TEST(pbox.get_input() == to_dynamic_bitset(data_in_2, bit_length));
    BOOST_TEST(pbox.get_output() == to_dynamic_bitset(0U, bit_length));
}

BOOST_AUTO_TEST_CASE(determine_next_after_set_input_reset) {
    size_t bit_length = bit_perm.size();
    PBox pbox(bit_length, bit_perm);

    unsigned int data_in_1 = 123;
    unsigned int data_in_2 = 1719;
    unsigned int data_out = 2775;
    pbox.set_input(to_dynamic_bitset(data_in_1, bit_length), BitsRange(0, bit_length));
    pbox.determine_next();
    pbox.set_input(to_dynamic_bitset(data_in_2, bit_length), BitsRange(0, bit_length));
    pbox.determine_next();

    BOOST_TEST(pbox.is_determined() == true);
    BOOST_TEST(pbox.get_probability() == 1.0);
    BOOST_TEST(pbox.get_output() == to_dynamic_bitset(data_out, bit_length));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_identitybox)

BOOST_AUTO_TEST_CASE(constructor) {
    size_t bit_length = 16;
    IdentityBox ibox(bit_length);

    BOOST_TEST(ibox.get_input().size() == bit_length);
    BOOST_TEST(ibox.get_output().size() == bit_length);
    BOOST_TEST(ibox.is_determined() == false);
    BOOST_TEST(ibox.get_probability() == 0.0);
    BOOST_TEST(ibox.get_input() == to_dynamic_bitset(0U, bit_length));
    BOOST_TEST(ibox.get_output() == to_dynamic_bitset(0U, bit_length));
}

BOOST_AUTO_TEST_CASE(input_get_set_simple) {
    size_t bit_length = 16;
    IdentityBox ibox(bit_length);

    unsigned int data_in = 48395;
    ibox.set_input(to_dynamic_bitset(data_in, bit_length), BitsRange(0, bit_length));

    BOOST_TEST(ibox.get_input() == to_dynamic_bitset(data_in, bit_length));
}

BOOST_AUTO_TEST_CASE(input_get_set_complex) {
    size_t bit_length = 16;
    IdentityBox ibox(bit_length);

    BOOST_REQUIRE(bit_length % 2 == 0);

    unsigned int first_half = 235;
    unsigned int second_half = 195;
    ibox.set_input(to_dynamic_bitset(first_half, bit_length / 2), BitsRange(0, bit_length / 2));
    ibox.set_input(to_dynamic_bitset(second_half, bit_length / 2), BitsRange(bit_length / 2, bit_length / 2));

    BOOST_TEST(ibox.get_input() == to_dynamic_bitset(first_half + (second_half << (bit_length / 2)), bit_length));
}

BOOST_AUTO_TEST_CASE(determine_next_idempotent) {
    size_t bit_length = 16;
    unsigned int data_in_out = 17893;
    IdentityBox ibox(bit_length);

    ibox.set_input(to_dynamic_bitset(data_in_out, bit_length), BitsRange(0, bit_length));
    ibox.determine_next();

    BOOST_TEST(ibox.is_determined());
    BOOST_TEST(ibox.get_probability() == 1.0);
    BOOST_TEST(ibox.get_output() == to_dynamic_bitset(data_in_out, bit_length));

    ibox.determine_next();

    BOOST_TEST(ibox.is_determined());
    BOOST_TEST(ibox.get_probability() == 1.0);
    BOOST_TEST(ibox.get_output() == to_dynamic_bitset(data_in_out, bit_length));
}

BOOST_AUTO_TEST_CASE(set_input_after_determine_next) {
    size_t bit_length = 16;
    IdentityBox ibox(bit_length);

    unsigned int data_in_1 = 16853;
    unsigned int data_in_2 = 63215;
    ibox.set_input(to_dynamic_bitset(data_in_1, bit_length), BitsRange(0, bit_length));
    ibox.determine_next();
    ibox.set_input(to_dynamic_bitset(data_in_2, bit_length), BitsRange(0, bit_length));

    BOOST_TEST(ibox.is_determined() == false);
    BOOST_TEST(ibox.get_probability() == 0.0);
    BOOST_TEST(ibox.get_input() == to_dynamic_bitset(data_in_2, bit_length));
    BOOST_TEST(ibox.get_output() == to_dynamic_bitset(0U, bit_length));
}

BOOST_AUTO_TEST_CASE(determine_next_after_set_input_reset) {
    size_t bit_length = 16;
    IdentityBox ibox(bit_length);

    unsigned int data_in_1 = 16853;
    unsigned int data_in_out = 63215;
    ibox.set_input(to_dynamic_bitset(data_in_1, bit_length), BitsRange(0, bit_length));
    ibox.determine_next();
    ibox.set_input(to_dynamic_bitset(data_in_out, bit_length), BitsRange(0, bit_length));
    ibox.determine_next();

    BOOST_TEST(ibox.is_determined() == true);
    BOOST_TEST(ibox.get_probability() == 1.0);
    BOOST_TEST(ibox.get_output() == to_dynamic_bitset(data_in_out, bit_length));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_xorbox)

BOOST_AUTO_TEST_CASE(constructor) {
    size_t bit_length = 16;
    XorBox xorbox(bit_length);

    BOOST_TEST(xorbox.get_input().size() == 2 * bit_length);
    BOOST_TEST(xorbox.get_output().size() == bit_length);
    BOOST_TEST(xorbox.is_determined() == false);
    BOOST_TEST(xorbox.get_probability() == 0.0);
    BOOST_TEST(xorbox.get_input() == to_dynamic_bitset(0U, 2 * bit_length));
    BOOST_TEST(xorbox.get_output() == to_dynamic_bitset(0U, bit_length));
}

BOOST_AUTO_TEST_CASE(input_get_set_simple) {
    size_t bit_length = 16;
    XorBox xorbox(bit_length);

    unsigned int data_in_1 = 35702;
    unsigned int data_in_2 = 44217;
    xorbox.set_input(to_dynamic_bitset(data_in_1, bit_length), BitsRange(0, bit_length));
    xorbox.set_input(to_dynamic_bitset(data_in_2, bit_length), BitsRange(bit_length, bit_length));

    BOOST_TEST(xorbox.get_input() == to_dynamic_bitset(data_in_1 + (data_in_2 << bit_length), 2 * bit_length));
}

BOOST_AUTO_TEST_CASE(input_get_set_complex) {
    size_t bit_length = 16;
    XorBox xorbox(bit_length);

    BOOST_REQUIRE(bit_length % 2 == 0);

    unsigned int first_half_1 = 235;
    unsigned int second_half_1 = 195;
    unsigned int first_half_2 = 42;
    unsigned int second_half_2 = 67;
    xorbox.set_input(to_dynamic_bitset(first_half_1, bit_length / 2), BitsRange(0, bit_length / 2));
    xorbox.set_input(to_dynamic_bitset(second_half_1, bit_length / 2), BitsRange(bit_length / 2, bit_length / 2));
    xorbox.set_input(to_dynamic_bitset(first_half_2, bit_length / 2), BitsRange(bit_length, bit_length / 2));
    xorbox.set_input(to_dynamic_bitset(second_half_2, bit_length / 2), BitsRange(3 * bit_length / 2, bit_length / 2));

    unsigned int data_1 = first_half_1 + (second_half_1 << (bit_length / 2));
    unsigned int data_2 = first_half_2 + (second_half_2 << (bit_length / 2));
    BOOST_TEST(xorbox.get_input() == to_dynamic_bitset(data_1 + (data_2 << bit_length), 2 * bit_length));
}

BOOST_AUTO_TEST_CASE(determine_next_idempotent) {
    size_t bit_length = 16;
    unsigned int data_in_1 = 35702;
    unsigned int data_in_2 = 44217;
    XorBox xorbox(bit_length);

    xorbox.set_input(to_dynamic_bitset(data_in_1, bit_length), BitsRange(0, bit_length));
    xorbox.set_input(to_dynamic_bitset(data_in_2, bit_length), BitsRange(bit_length, bit_length));
    xorbox.determine_next();

    BOOST_TEST(xorbox.is_determined());
    BOOST_TEST(xorbox.get_probability() == 1.0);
    BOOST_TEST(xorbox.get_output() == to_dynamic_bitset(data_in_1 ^ data_in_2, bit_length));

    xorbox.determine_next();

    BOOST_TEST(xorbox.is_determined());
    BOOST_TEST(xorbox.get_probability() == 1.0);
    BOOST_TEST(xorbox.get_output() == to_dynamic_bitset(data_in_1 ^ data_in_2, bit_length));
}

BOOST_AUTO_TEST_CASE(set_input_after_determine_next) {
    size_t bit_length = 16;
    XorBox xorbox(bit_length);

    unsigned int data_in_1 = 35702;
    unsigned int data_in_2 = 44217;
    unsigned int data_in_3 = 16853;
    unsigned int data_in_4 = 63215;
    xorbox.set_input(to_dynamic_bitset(data_in_1, bit_length), BitsRange(0, bit_length));
    xorbox.set_input(to_dynamic_bitset(data_in_2, bit_length), BitsRange(bit_length, bit_length));
    xorbox.determine_next();
    xorbox.set_input(to_dynamic_bitset(data_in_3, bit_length), BitsRange(0, bit_length));
    xorbox.set_input(to_dynamic_bitset(data_in_4, bit_length), BitsRange(bit_length, bit_length));

    BOOST_TEST(xorbox.is_determined() == false);
    BOOST_TEST(xorbox.get_probability() == 0.0);
    BOOST_TEST(xorbox.get_input() == to_dynamic_bitset(data_in_3 + (data_in_4 << bit_length), 2 * bit_length));
    BOOST_TEST(xorbox.get_output() == to_dynamic_bitset(0U, bit_length));
}

BOOST_AUTO_TEST_CASE(determine_next_after_set_input_reset) {
    size_t bit_length = 16;
    XorBox xorbox(bit_length);

    unsigned int data_in_1 = 35702;
    unsigned int data_in_2 = 44217;
    unsigned int data_in_3 = 16853;
    unsigned int data_in_4 = 63215;
    xorbox.set_input(to_dynamic_bitset(data_in_1, bit_length), BitsRange(0, bit_length));
    xorbox.set_input(to_dynamic_bitset(data_in_2, bit_length), BitsRange(bit_length, bit_length));
    xorbox.determine_next();
    xorbox.set_input(to_dynamic_bitset(data_in_3, bit_length), BitsRange(0, bit_length));
    xorbox.set_input(to_dynamic_bitset(data_in_4, bit_length), BitsRange(bit_length, bit_length));
    xorbox.determine_next();

    BOOST_TEST(xorbox.is_determined() == true);
    BOOST_TEST(xorbox.get_probability() == 1.0);
    BOOST_TEST(xorbox.get_input() == to_dynamic_bitset(data_in_3 + (data_in_4 << bit_length), 2 * bit_length));
    BOOST_TEST(xorbox.get_output() == to_dynamic_bitset(data_in_3 ^ data_in_4, bit_length));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_sbox)

BOOST_AUTO_TEST_CASE(bad_constructor) {
    size_t bit_length_in = popcnt(s_box.size() - 1);
    size_t bit_length_out = popcnt(*max_element(s_box.begin(), s_box.end()));

    BOOST_CHECK_THROW(SBox sbox(bit_length_in - 1, bit_length_out, compute_diff_dist_table(s_box)), std::logic_error);
    BOOST_CHECK_THROW(SBox sbox(bit_length_in, bit_length_out - 1, compute_diff_dist_table(s_box)), std::logic_error);
}

BOOST_AUTO_TEST_CASE(constructor) {
    size_t bit_length_in = popcnt(s_box.size() - 1);
    size_t bit_length_out = popcnt(*max_element(s_box.begin(), s_box.end()));
    SBox sbox(bit_length_in, bit_length_out, compute_diff_dist_table(s_box));

    BOOST_TEST(sbox.get_input().size() == bit_length_in);
    BOOST_TEST(sbox.get_output().size() == bit_length_out);
    BOOST_TEST(sbox.is_determined() == false);
    BOOST_TEST(sbox.get_probability() == 0.0);
    BOOST_TEST(sbox.get_input() == to_dynamic_bitset(0U, bit_length_in));
    BOOST_TEST(sbox.get_output() == to_dynamic_bitset(0U, bit_length_out));
}

BOOST_AUTO_TEST_CASE(input_get_set_simple) {
    size_t bit_length_in = popcnt(s_box.size() - 1);
    size_t bit_length_out = popcnt(*max_element(s_box.begin(), s_box.end()));
    SBox sbox(bit_length_in, bit_length_out, compute_diff_dist_table(s_box));

    unsigned int data_in = 0xe;
    sbox.set_input(to_dynamic_bitset(data_in, bit_length_in), BitsRange(0, bit_length_in));

    BOOST_TEST(sbox.get_input() == to_dynamic_bitset(data_in, bit_length_in));
}

BOOST_AUTO_TEST_CASE(input_get_set_complex) {
    size_t bit_length_in = popcnt(s_box.size() - 1);
    size_t bit_length_out = popcnt(*max_element(s_box.begin(), s_box.end()));
    SBox sbox(bit_length_in, bit_length_out, compute_diff_dist_table(s_box));

    BOOST_REQUIRE(bit_length_in % 2 == 0);

    unsigned int first_half = 3;
    unsigned int second_half = 2;
    sbox.set_input(to_dynamic_bitset(first_half, bit_length_in / 2), BitsRange(0, bit_length_in / 2));
    sbox.set_input(to_dynamic_bitset(second_half, bit_length_in / 2), BitsRange(bit_length_in / 2, bit_length_in / 2));

    BOOST_TEST(sbox.get_input() == to_dynamic_bitset(first_half + (second_half << (bit_length_in / 2)), bit_length_in));
}

BOOST_AUTO_TEST_CASE(determine_next) {
    size_t bit_length_in = popcnt(s_box.size() - 1);
    size_t bit_length_out = popcnt(*max_element(s_box.begin(), s_box.end()));
    SBox sbox(bit_length_in, bit_length_out, compute_diff_dist_table(s_box));

    unsigned int data_in = 0x4;
    sbox.set_input(to_dynamic_bitset(data_in, bit_length_in), BitsRange(0, bit_length_in));

    sbox.determine_next();
    BOOST_TEST(sbox.is_determined() == false);
    BOOST_TEST(sbox.get_probability() == 6. / 16);
    BOOST_TEST(sbox.get_output() == to_dynamic_bitset(0x6U, bit_length_out));

    sbox.determine_next();
    BOOST_TEST(sbox.is_determined() == false);
    BOOST_TEST(sbox.get_probability() == 4. / 16);
    BOOST_TEST(sbox.get_output() == to_dynamic_bitset(0xbU, bit_length_out));

    sbox.determine_next();
    BOOST_TEST(sbox.is_determined() == false);
    BOOST_TEST(sbox.get_probability() == 2. / 16);
    BOOST_TEST(sbox.get_output() == to_dynamic_bitset(0x3U, bit_length_out));

    sbox.determine_next();
    BOOST_TEST(sbox.is_determined() == false);
    BOOST_TEST(sbox.get_probability() == 2. / 16);
    BOOST_TEST(sbox.get_output() == to_dynamic_bitset(0x9U, bit_length_out));

    sbox.determine_next();
    BOOST_TEST(sbox.is_determined() == true);
    BOOST_TEST(sbox.get_probability() == 2. / 16);
    BOOST_TEST(sbox.get_output() == to_dynamic_bitset(0xcU, bit_length_out));
}

BOOST_AUTO_TEST_CASE(determine_next_after_set_input_reset) {
    size_t bit_length_in = popcnt(s_box.size() - 1);
    size_t bit_length_out = popcnt(*max_element(s_box.begin(), s_box.end()));
    SBox sbox(bit_length_in, bit_length_out, compute_diff_dist_table(s_box));

    unsigned int data_in_1 = 0x4;
    unsigned int data_in_2 = 0xf;
    sbox.set_input(to_dynamic_bitset(data_in_1, bit_length_in), BitsRange(0, bit_length_in));

    sbox.determine_next();
    BOOST_TEST(sbox.is_determined() == false);
    BOOST_TEST(sbox.get_probability() == 6. / 16);
    BOOST_TEST(sbox.get_output() == to_dynamic_bitset(0x6U, bit_length_out));

    sbox.set_input(to_dynamic_bitset(data_in_2, bit_length_in), BitsRange(0, bit_length_in));

    BOOST_TEST(sbox.is_determined() == false);
    BOOST_TEST(sbox.get_probability() == 0.0);
    BOOST_TEST(sbox.get_output() == to_dynamic_bitset(0U, bit_length_out));

    sbox.determine_next();
    BOOST_TEST(sbox.is_determined() == false);
    BOOST_TEST(sbox.get_probability() == 6. / 16);
    BOOST_TEST(sbox.get_output() == to_dynamic_bitset(0x4U, bit_length_out));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_box_interaction)

BOOST_AUTO_TEST_CASE(constructor_with_null_dst_box) {
    size_t bit_length_ibox = 8;
    const vector<pair<AbstractBoxPtr, Connection>> bad_dst_boxes{{nullptr, {BitsRange(0, 4), BitsRange(0, 4)}}};
    BOOST_CHECK_THROW(IdentityBox ibox(bit_length_ibox, bad_dst_boxes), std::logic_error);
}

BOOST_AUTO_TEST_CASE(constructor_with_bad_connection) {
    size_t bit_length_xorbox = 4;
    size_t bit_length_ibox = 8;

    XorBoxPtr xorbox_ptr = std::make_shared<XorBox>(bit_length_xorbox);
    const vector<pair<AbstractBoxPtr, Connection>> bad_dst_boxes_1{{xorbox_ptr, {BitsRange(0, 5), BitsRange(0, 4)}}};
    const vector<pair<AbstractBoxPtr, Connection>> bad_dst_boxes_2{{xorbox_ptr, {BitsRange(4, 8), BitsRange(0, 8)}}};
    const vector<pair<AbstractBoxPtr, Connection>> bad_dst_boxes_3{{xorbox_ptr, {BitsRange(0, 8), BitsRange(4, 8)}}};

    BOOST_CHECK_THROW(IdentityBox ibox(bit_length_ibox, bad_dst_boxes_1), std::logic_error);
    BOOST_CHECK_THROW(IdentityBox ibox(bit_length_ibox, bad_dst_boxes_2), std::logic_error);
    BOOST_CHECK_THROW(IdentityBox ibox(bit_length_ibox, bad_dst_boxes_3), std::logic_error);
}

BOOST_AUTO_TEST_CASE(constructor_with_dst_boxes) {
    size_t bit_length_sbox_in = popcnt(s_box.size() - 1);
    size_t bit_length_sbox_out = popcnt(*max_element(s_box.begin(), s_box.end()));
    size_t bit_length_sbox = 4;
    size_t bit_length_ibox = 8;

    SBoxPtr sbox_ptr_1 =
        std::make_shared<SBox>(bit_length_sbox_in, bit_length_sbox_out, compute_diff_dist_table(s_box));
    SBoxPtr sbox_ptr_2 =
        std::make_shared<SBox>(bit_length_sbox_in, bit_length_sbox_out, compute_diff_dist_table(s_box));
    const vector<pair<AbstractBoxPtr, Connection>> dst_boxes{{sbox_ptr_1, {BitsRange(0, 4), BitsRange(0, 4)}},
                                                             {sbox_ptr_2, {BitsRange(4, 4), BitsRange(0, 4)}}};

    BOOST_CHECK_NO_THROW(IdentityBox ibox(bit_length_ibox, dst_boxes));
}

BOOST_AUTO_TEST_CASE(bad_add_dest) {
    size_t bit_length_sbox_in = popcnt(s_box.size() - 1);
    size_t bit_length_sbox_out = popcnt(*max_element(s_box.begin(), s_box.end()));
    size_t bit_length_sbox = 4;
    size_t bit_length_ibox = 8;

    SBoxPtr sbox_ptr = std::make_shared<SBox>(bit_length_sbox_in, bit_length_sbox_out, compute_diff_dist_table(s_box));

    IdentityBox ibox(bit_length_ibox);

    BOOST_CHECK_THROW(ibox.add_dest(nullptr, BitsRange(0, 4), BitsRange(0, 4)), std::logic_error);
    BOOST_CHECK_THROW(ibox.add_dest(sbox_ptr, BitsRange(0, 5), BitsRange(0, 4)), std::logic_error);
    BOOST_CHECK_THROW(ibox.add_dest(sbox_ptr, BitsRange(8, 4), BitsRange(0, 4)), std::logic_error);
    BOOST_CHECK_THROW(ibox.add_dest(sbox_ptr, BitsRange(0, 4), BitsRange(8, 4)), std::logic_error);
}

BOOST_AUTO_TEST_CASE(add_dest) {
    size_t bit_length_sbox_in = popcnt(s_box.size() - 1);
    size_t bit_length_sbox_out = popcnt(*max_element(s_box.begin(), s_box.end()));
    size_t bit_length_sbox = 4;
    size_t bit_length_ibox = 8;

    SBoxPtr sbox_ptr_1 =
        std::make_shared<SBox>(bit_length_sbox_in, bit_length_sbox_out, compute_diff_dist_table(s_box));
    SBoxPtr sbox_ptr_2 =
        std::make_shared<SBox>(bit_length_sbox_in, bit_length_sbox_out, compute_diff_dist_table(s_box));

    IdentityBox ibox(bit_length_ibox);

    BOOST_CHECK_NO_THROW(ibox.add_dest(sbox_ptr_1, BitsRange(0, 4), BitsRange(0, 4)));
    BOOST_CHECK_NO_THROW(ibox.add_dest(sbox_ptr_2, BitsRange(4, 4), BitsRange(0, 4)));
}

BOOST_AUTO_TEST_CASE(notify_all) {
    size_t bit_length_sbox_in = popcnt(s_box.size() - 1);
    size_t bit_length_sbox_out = popcnt(*max_element(s_box.begin(), s_box.end()));
    size_t bit_length_sbox = 4;
    size_t bit_length_ibox = 8;

    SBoxPtr sbox_ptr_1 =
        std::make_shared<SBox>(bit_length_sbox_in, bit_length_sbox_out, compute_diff_dist_table(s_box));
    SBoxPtr sbox_ptr_2 =
        std::make_shared<SBox>(bit_length_sbox_in, bit_length_sbox_out, compute_diff_dist_table(s_box));

    IdentityBoxPtr ibox_in_ptr = std::make_shared<IdentityBox>(bit_length_ibox);
    IdentityBoxPtr ibox_out_ptr = std::make_shared<IdentityBox>(bit_length_ibox);

    ibox_in_ptr->add_dest(sbox_ptr_1, BitsRange(0, 4), BitsRange(0, 4));
    ibox_in_ptr->add_dest(sbox_ptr_2, BitsRange(4, 4), BitsRange(0, 4));

    sbox_ptr_1->add_dest(ibox_out_ptr, BitsRange(0, 4), BitsRange(0, 4));
    sbox_ptr_2->add_dest(ibox_out_ptr, BitsRange(0, 4), BitsRange(4, 4));

    unsigned int sbox_1_in = 0xe;
    unsigned int sbox_2_in = 0x4;
    unsigned int sbox_1_out = 0x8;
    unsigned int sbox_2_out = 0x6;
    unsigned int ibox_in = sbox_1_in + (sbox_2_in << bit_length_sbox);
    unsigned int ibox_out = sbox_1_out + (sbox_2_out << bit_length_sbox);

    ibox_in_ptr->set_input(to_dynamic_bitset(ibox_in, bit_length_ibox), BitsRange(0, 8));
    ibox_in_ptr->determine_next();

    BOOST_TEST(ibox_in_ptr->get_output() == to_dynamic_bitset(ibox_in, bit_length_ibox));

    ibox_in_ptr->notify_all();

    BOOST_TEST(sbox_ptr_1->get_input() == to_dynamic_bitset(sbox_1_in, bit_length_sbox));
    BOOST_TEST(sbox_ptr_2->get_input() == to_dynamic_bitset(sbox_2_in, bit_length_sbox));

    sbox_ptr_1->determine_next();
    sbox_ptr_2->determine_next();

    BOOST_TEST(sbox_ptr_1->get_output() == to_dynamic_bitset(sbox_1_out, bit_length_sbox));
    BOOST_TEST(sbox_ptr_2->get_output() == to_dynamic_bitset(sbox_2_out, bit_length_sbox));

    sbox_ptr_1->notify_all();
    sbox_ptr_2->notify_all();

    BOOST_TEST(ibox_out_ptr->get_input() == to_dynamic_bitset(ibox_out, bit_length_ibox));
}

BOOST_AUTO_TEST_SUITE_END()
