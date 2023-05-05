#include "src/box/sbox.h"
#include "src/box/xorbox.h"
#include "src/cipheranalyzer.h"
#include "src/helpers/helpers.h"
#include "src/roundfunction.h"
#include <boost/dynamic_bitset.hpp>
#include <cstdio>

using namespace std;
using namespace boost;

CipherAnalyzerPtr create_cipher() {
    AbstractBoxConstructor sbox_constructor = []() {
        vector<size_t> sbox{0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8,
                            0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7};
        return make_shared<SBox>(4, 4, compute_differential_table(sbox));
    };

    AbstractBoxConstructor pbox_constructor = []() {
        vector<size_t> pbox{0x0, 0x4, 0x8, 0xc, 0x1, 0x5, 0x9, 0xd,
                            0x2, 0x6, 0xa, 0xe, 0x3, 0x7, 0xb, 0xf};
        return make_shared<PBox>(16, pbox);
    };

    AbstractBoxConstructor identity_constructor = []() {
        return make_shared<IdentityBox>(16);
    };

    map<string, AbstractBoxConstructor> constrs{
        {"Src", identity_constructor}, {"Sbox1", sbox_constructor},
        {"Sbox2", sbox_constructor},   {"Sbox3", sbox_constructor},
        {"Sbox4", sbox_constructor},   {"Pbox", pbox_constructor}};

    map<string, vector<NamedConnection>> conns{
        {"Src",
         {
             {"Sbox1", {{0, 4}, {0, 4}}},
             {"Sbox2", {{4, 4}, {0, 4}}},
             {"Sbox3", {{8, 4}, {0, 4}}},
             {"Sbox4", {{12, 4}, {0, 4}}},
         }},
        {"Sbox1", {{"Pbox", {{0, 4}, {0, 4}}}}},
        {"Sbox2", {{"Pbox", {{0, 4}, {4, 4}}}}},
        {"Sbox3", {{"Pbox", {{0, 4}, {8, 4}}}}},
        {"Sbox4", {{"Pbox", {{0, 4}, {12, 4}}}}}};

    vector<RoundFunctionPtr> rounds{
        make_shared<RoundFunction>("Src", "Pbox", constrs, conns),
        make_shared<RoundFunction>("Src", "Pbox", constrs, conns),
        make_shared<RoundFunction>("Src", "Pbox", constrs, conns)};

    return make_shared<CipherAnalyzer>(rounds, 16, 0.0);
}

int main() {
    CipherAnalyzerPtr cipher = create_cipher();

    for (unsigned i = 1; i < 16; i++) {
        auto input = dynamic_bitset<>(16);
        input.set(0, i, 1);

        do {
            cipher->set_input(input, {0, 16});
            auto [output, prob] = cipher->get_next_entry();

            while (output.size() > 0) {
                unsigned int inp = to_uint(input);
                unsigned int out = to_uint(output);

                printf("in: 0x%04x (%d),\tout: 0x%04x (%d)\t->\tprob %.6lf\n",
                       inp, __builtin_popcount(inp), out,
                       __builtin_popcount(out), prob);

                auto [next_output, next_prob] = cipher->get_next_entry();
                output = next_output, prob = next_prob;
            }
        } while (increment_bits(input));
    }

    return 0;
}
