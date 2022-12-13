#include "cipheranalyzer.h"
#include "helpers.h"
#include "roundfunction.h"
#include "sbox.h"
#include <boost/dynamic_bitset.hpp>
#include <iostream>

using namespace std;
using namespace boost;

unsigned int to_uint(const dynamic_bitset<> &bitset) {
    int ans = 0;
    for (ssize_t i = bitset.size() - 1; i >= 0; i--) {
        ans = (ans << 1) + bitset[i];
    }
    return ans;
}

dynamic_bitset<> from_uint(unsigned int x, int size) {
    dynamic_bitset<> result(size);
    size_t idx = 0;
    while (x) {
        result[idx++] = x & 1;
        x >>= 1;
    }

    return result;
}

int main() {
    function<std::shared_ptr<AbstractBox>()> sbox_constructor = []() {
        vector<size_t> sbox{14, 4,  13, 1,  2, 15, 11, 8,
                            3,  10, 6,  12, 5, 9,  0,  7};
        return std::make_shared<SBox>(
            SBox(4, 4, compute_differential_table(sbox)));
    };
    function<std::shared_ptr<AbstractBox>()> pbox_constructor = []() {
        vector<size_t> pbox{0, 4, 8,  12, 1, 5, 9,  13,
                            2, 6, 10, 14, 3, 7, 11, 15};
        return std::make_shared<PBox>(PBox(16, pbox));
    };

    function<std::shared_ptr<AbstractBox>()> identity_constructor = []() {
        return std::make_shared<IdentityBox>(IdentityBox(16));
    };

    map<string, function<std::shared_ptr<AbstractBox>()>> constructors{
        {"Src", identity_constructor}, {"Sbox1", sbox_constructor},
        {"Sbox2", sbox_constructor},   {"Sbox3", sbox_constructor},
        {"Sbox4", sbox_constructor},   {"Pbox", pbox_constructor}};

    map<string, vector<pair<string, pair<bits_range, bits_range>>>> connections{
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
    vector<std::shared_ptr<RoundFunction>> rounds{
        std::make_shared<RoundFunction>("Src", "Pbox", constructors,
                                        connections),
        std::make_shared<RoundFunction>("Src", "Pbox", constructors,
                                        connections),
        std::make_shared<RoundFunction>("Src", "Pbox", constructors,
                                        connections)};

    std::shared_ptr<CipherAnalyzer> cipher =
        std::make_shared<CipherAnalyzer>(rounds, 0.0);
    for (unsigned i = 1; i < 16; i++) {
        auto input = dynamic_bitset<>(16);
        input.set(0, i, 1);
        bool done = false;
        while (!done) {
            cipher->set_input(input, {0, 16});
            auto diff = cipher->get_next_differential();
            while (diff.first.size() > 0) {
                auto output = diff.first;
                auto prob = diff.second;
                unsigned int inp = to_uint(input), out = to_uint(output);
                cout << inp << "(" << __builtin_popcount(inp) << "), " << out
                     << "(" << __builtin_popcount(out) << "), " << prob << ", "
                     << 1 / prob << "\n";
                diff = cipher->get_next_differential();
            }
            done = !increment_bits(input);
        }
    }

    return 0;
}
