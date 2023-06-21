#include "src/box/sbox.h"
#include "src/box/xorbox.h"
#include "src/cipheranalyzer.h"
#include "src/helpers/helpers.h"
#include "src/roundfunction.h"
#include <boost/dynamic_bitset.hpp>
#include <builders/cipheranalyzerbuilder.h>
#include <builders/eboxbuilder.h>
#include <builders/identityboxbuilder.h>
#include <builders/pboxbuilder.h>
#include <builders/sboxbuilder.h>
#include <builders/xorboxbuilder.h>
#include <cstdio>
#include <examples/des_boxes.h>

using namespace std;
using namespace boost;

const int max_weight = 6;

CipherAnalyzerBuilderPtr create_cipher_builder() {
    AbstractBoxBuilderPtr ebox_constr = make_ebox_builder(bit_expansion);
    AbstractBoxBuilderPtr pbox_constr = make_pbox_builder(bit_perm);

    AbstractBoxBuilderPtr sbox_constr_1 = make_sbox_builder(6, 4, compute_diff_dist_table(sbox1), true);
    AbstractBoxBuilderPtr sbox_constr_2 = make_sbox_builder(6, 4, compute_diff_dist_table(sbox2), true);
    AbstractBoxBuilderPtr sbox_constr_3 = make_sbox_builder(6, 4, compute_diff_dist_table(sbox3), true);
    AbstractBoxBuilderPtr sbox_constr_4 = make_sbox_builder(6, 4, compute_diff_dist_table(sbox4), true);
    AbstractBoxBuilderPtr sbox_constr_5 = make_sbox_builder(6, 4, compute_diff_dist_table(sbox5), true);
    AbstractBoxBuilderPtr sbox_constr_6 = make_sbox_builder(6, 4, compute_diff_dist_table(sbox6), true);
    AbstractBoxBuilderPtr sbox_constr_7 = make_sbox_builder(6, 4, compute_diff_dist_table(sbox7), true);
    AbstractBoxBuilderPtr sbox_constr_8 = make_sbox_builder(6, 4, compute_diff_dist_table(sbox8), true);

    RoundBuilderPtr round_builder = make_round_builder();
    round_builder->add_builder("E", ebox_constr);
    round_builder->add_builder("P", pbox_constr);
    round_builder->add_builder("S1", sbox_constr_1);
    round_builder->add_builder("S2", sbox_constr_2);
    round_builder->add_builder("S3", sbox_constr_3);
    round_builder->add_builder("S4", sbox_constr_4);
    round_builder->add_builder("S5", sbox_constr_5);
    round_builder->add_builder("S6", sbox_constr_6);
    round_builder->add_builder("S7", sbox_constr_7);
    round_builder->add_builder("S8", sbox_constr_8);

    round_builder->add_connection("E", "S8", BitsRange(0, 6), BitsRange(0, 6));
    round_builder->add_connection("E", "S7", BitsRange(6, 6), BitsRange(0, 6));
    round_builder->add_connection("E", "S6", BitsRange(12, 6), BitsRange(0, 6));
    round_builder->add_connection("E", "S5", BitsRange(18, 6), BitsRange(0, 6));
    round_builder->add_connection("E", "S4", BitsRange(24, 6), BitsRange(0, 6));
    round_builder->add_connection("E", "S3", BitsRange(30, 6), BitsRange(0, 6));
    round_builder->add_connection("E", "S2", BitsRange(36, 6), BitsRange(0, 6));
    round_builder->add_connection("E", "S1", BitsRange(42, 6), BitsRange(0, 6));

    round_builder->add_connection("S8", "P", BitsRange(0, 4), BitsRange(0, 4));
    round_builder->add_connection("S7", "P", BitsRange(0, 4), BitsRange(4, 4));
    round_builder->add_connection("S6", "P", BitsRange(0, 4), BitsRange(8, 4));
    round_builder->add_connection("S5", "P", BitsRange(0, 4), BitsRange(12, 4));
    round_builder->add_connection("S4", "P", BitsRange(0, 4), BitsRange(16, 4));
    round_builder->add_connection("S3", "P", BitsRange(0, 4), BitsRange(20, 4));
    round_builder->add_connection("S2", "P", BitsRange(0, 4), BitsRange(24, 4));
    round_builder->add_connection("S1", "P", BitsRange(0, 4), BitsRange(28, 4));

    round_builder->set_src("E");
    round_builder->set_dst("P");

    CipherAnalyzerBuilderPtr cipher_builder = make_cipher_analyzer_builder();
    cipher_builder->add_round_builder("Round", round_builder);
    cipher_builder->add_round_name("Round");

    return cipher_builder;
}

int main() {
    CipherAnalyzerBuilderPtr cipher_builder = create_cipher_builder();
    CipherAnalyzerPtr cipher = cipher_builder->build();

    for (unsigned i = 1; i <= max_weight; i++) {
        auto input = dynamic_bitset<>(cipher->get_input_size());
        input.set(0, i, 1);
        printf("Trying with %d bits\n", i);

        do {
            cipher->set_input(input, BitsRange(0, cipher->get_input_size()));
            auto [output, prob] = cipher->get_next_state();

            while (output.size() > 0) {
                if (prob > 0) {
                    printf("in: 0x%s (%ld),\tout: 0x%s (%ld) -> prob %.16lf\n", convert_to_hex_string(input).c_str(),
                           input.count(), convert_to_hex_string(output).c_str(), output.count(), prob);
                }

                auto [next_output, next_prob] = cipher->get_next_state();
                output = next_output, prob = next_prob;
            }
        } while (increment_bits(input));
    }

    return 0;
}
