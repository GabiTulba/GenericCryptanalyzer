#include "src/box/sbox.h"
#include "src/box/xorbox.h"
#include "src/cipheranalyzer.h"
#include "src/helpers/helpers.h"
#include "src/roundfunction.h"
#include <boost/dynamic_bitset.hpp>
#include <builders/abstractboxbuilder.h>
#include <builders/cipheranalyzerbuilder.h>
#include <builders/identityboxbuilder.h>
#include <builders/pboxbuilder.h>
#include <builders/roundbuilder.h>
#include <builders/sboxbuilder.h>
#include <cstdio>

using namespace std;
using namespace boost;

const int round_cnt = 5;
const int max_weight = 6;
const int max_queue_size = 1 << 20;
const int threads = 24;
const int max_inputs = 16;

CipherAnalyzerBuilderPtr create_cipher_builder() {
    vector<size_t> sbox{0xc, 0x5, 0x6, 0xb, 0x9, 0x0, 0xa, 0xd, 0x3, 0xe, 0xf, 0x8, 0x4, 0x7, 0x1, 0x2};
    vector<size_t> pbox{0,  16, 32, 48, 1,  17, 33, 49, 2,  18, 34, 50, 3,  19, 35, 51, 4,  20, 36, 52, 5,  21,
                        37, 53, 6,  22, 38, 54, 7,  23, 39, 55, 8,  24, 40, 56, 9,  25, 41, 57, 10, 26, 42, 58,
                        11, 27, 43, 59, 12, 28, 44, 60, 13, 29, 45, 61, 14, 30, 46, 62, 15, 31, 47, 63};

    AbstractBoxBuilderPtr pbox_builder = make_pbox_builder(pbox);
    AbstractBoxBuilderPtr sbox_builder = make_sbox_builder(sbox, true);
    AbstractBoxBuilderPtr identitybox_builder = make_identitybox_builder(64);

    RoundBuilderPtr round_builder = make_round_builder();
    round_builder->add_builder("Src", identitybox_builder);
    round_builder->add_builder("Sbox1", sbox_builder);
    round_builder->add_builder("Sbox2", sbox_builder);
    round_builder->add_builder("Sbox3", sbox_builder);
    round_builder->add_builder("Sbox4", sbox_builder);
    round_builder->add_builder("Sbox5", sbox_builder);
    round_builder->add_builder("Sbox6", sbox_builder);
    round_builder->add_builder("Sbox7", sbox_builder);
    round_builder->add_builder("Sbox8", sbox_builder);
    round_builder->add_builder("Sbox9", sbox_builder);
    round_builder->add_builder("Sbox10", sbox_builder);
    round_builder->add_builder("Sbox11", sbox_builder);
    round_builder->add_builder("Sbox12", sbox_builder);
    round_builder->add_builder("Sbox13", sbox_builder);
    round_builder->add_builder("Sbox14", sbox_builder);
    round_builder->add_builder("Sbox15", sbox_builder);
    round_builder->add_builder("Sbox16", sbox_builder);
    round_builder->add_builder("Pbox", pbox_builder);

    round_builder->add_connection("Src", "Sbox1", BitsRange(0, 4), BitsRange(0, 4));
    round_builder->add_connection("Src", "Sbox2", BitsRange(4, 4), BitsRange(0, 4));
    round_builder->add_connection("Src", "Sbox3", BitsRange(8, 4), BitsRange(0, 4));
    round_builder->add_connection("Src", "Sbox4", BitsRange(12, 4), BitsRange(0, 4));
    round_builder->add_connection("Src", "Sbox5", BitsRange(16, 4), BitsRange(0, 4));
    round_builder->add_connection("Src", "Sbox6", BitsRange(20, 4), BitsRange(0, 4));
    round_builder->add_connection("Src", "Sbox7", BitsRange(24, 4), BitsRange(0, 4));
    round_builder->add_connection("Src", "Sbox8", BitsRange(28, 4), BitsRange(0, 4));
    round_builder->add_connection("Src", "Sbox9", BitsRange(32, 4), BitsRange(0, 4));
    round_builder->add_connection("Src", "Sbox10", BitsRange(36, 4), BitsRange(0, 4));
    round_builder->add_connection("Src", "Sbox11", BitsRange(40, 4), BitsRange(0, 4));
    round_builder->add_connection("Src", "Sbox12", BitsRange(44, 4), BitsRange(0, 4));
    round_builder->add_connection("Src", "Sbox13", BitsRange(48, 4), BitsRange(0, 4));
    round_builder->add_connection("Src", "Sbox14", BitsRange(52, 4), BitsRange(0, 4));
    round_builder->add_connection("Src", "Sbox15", BitsRange(56, 4), BitsRange(0, 4));
    round_builder->add_connection("Src", "Sbox16", BitsRange(60, 4), BitsRange(0, 4));
    round_builder->add_connection("Sbox1", "Pbox", BitsRange(0, 4), BitsRange(0, 4));
    round_builder->add_connection("Sbox2", "Pbox", BitsRange(0, 4), BitsRange(4, 4));
    round_builder->add_connection("Sbox3", "Pbox", BitsRange(0, 4), BitsRange(8, 4));
    round_builder->add_connection("Sbox4", "Pbox", BitsRange(0, 4), BitsRange(12, 4));
    round_builder->add_connection("Sbox5", "Pbox", BitsRange(0, 4), BitsRange(16, 4));
    round_builder->add_connection("Sbox6", "Pbox", BitsRange(0, 4), BitsRange(20, 4));
    round_builder->add_connection("Sbox7", "Pbox", BitsRange(0, 4), BitsRange(24, 4));
    round_builder->add_connection("Sbox8", "Pbox", BitsRange(0, 4), BitsRange(28, 4));
    round_builder->add_connection("Sbox9", "Pbox", BitsRange(0, 4), BitsRange(32, 4));
    round_builder->add_connection("Sbox10", "Pbox", BitsRange(0, 4), BitsRange(36, 4));
    round_builder->add_connection("Sbox11", "Pbox", BitsRange(0, 4), BitsRange(40, 4));
    round_builder->add_connection("Sbox12", "Pbox", BitsRange(0, 4), BitsRange(44, 4));
    round_builder->add_connection("Sbox13", "Pbox", BitsRange(0, 4), BitsRange(48, 4));
    round_builder->add_connection("Sbox14", "Pbox", BitsRange(0, 4), BitsRange(52, 4));
    round_builder->add_connection("Sbox15", "Pbox", BitsRange(0, 4), BitsRange(56, 4));
    round_builder->add_connection("Sbox16", "Pbox", BitsRange(0, 4), BitsRange(60, 4));
    round_builder->set_src("Src");
    round_builder->set_dst("Pbox");

    CipherAnalyzerBuilderPtr cipher_builder = make_cipher_analyzer_builder();
    cipher_builder->add_round_builder("Round", round_builder);
    for (int i = 0; i < round_cnt; i++) {
        cipher_builder->add_round_name("Round");
    }
    return cipher_builder;
}

void job(CipherAnalyzerBuilderPtr &cipher_builder, CipherAnalyzerPtr &cipher, pthread_mutex_t *cipher_lock,
         dynamic_bitset<> &input, int thread_idx, sem_t *queue_semaphore) {
    pthread_mutex_lock(cipher_lock);

    cipher->set_input(input, BitsRange(0, cipher->get_input_size()));
    auto [output, prob] = cipher->get_next_state();

    while (output.size() > 0) {
        printf("[%02d] in: 0x%s (%02ld),\tout: 0x%s (%02ld) -> prob %.30lf\n", thread_idx,
               convert_to_hex_string(input).c_str(), input.count(), convert_to_hex_string(output).c_str(),
               output.count(), prob);

        cipher_builder->update_global_thresh(prob);

        auto [next_output, next_prob] = cipher->get_next_state();
        output = next_output, prob = next_prob;
    }

    sem_post(queue_semaphore);

    pthread_mutex_unlock(cipher_lock);
}

int main() {
    int thread_idx = 0;
    sem_t queue_semaphore;
    asio::thread_pool worker_pool(threads);
    CipherAnalyzerBuilderPtr cipher_builder = create_cipher_builder();
    vector<CipherAnalyzerPtr> ciphers;
    vector<pthread_mutex_t> cipher_locks(threads);
    vector<dynamic_bitset<>> initial_inputs;

    sem_init(&queue_semaphore, 0, max_queue_size);

    for (int i = 0; i < threads; i++) {
        ciphers.push_back(cipher_builder->build());
        pthread_mutex_init(&cipher_locks[i], NULL);
        if (i == 0) {
            initial_inputs = ciphers[0]->calculate_opt_probs(max_weight, max_inputs, threads, max_queue_size);
            cipher_builder->set_opt_probs(ciphers[0]->get_opt_probs());
        }
    }

    for (auto &initial_input : initial_inputs) {
        sem_wait(&queue_semaphore);
        asio::post(worker_pool, bind(job, cipher_builder, ciphers[thread_idx], &cipher_locks[thread_idx], initial_input,
                                     thread_idx, &queue_semaphore));
        thread_idx++;
        if (thread_idx == threads) {
            thread_idx = 0;
        }
    }

    for (unsigned i = 1; i <= max_weight; i++) {
        dynamic_bitset<> input = dynamic_bitset<>(ciphers[0]->get_input_size());
        input.set(0, i, 1);
        printf("Trying with %d bits\n", i);

        do {
            sem_wait(&queue_semaphore);

            asio::post(worker_pool, bind(job, cipher_builder, ciphers[thread_idx], &cipher_locks[thread_idx], input,
                                         thread_idx, &queue_semaphore));
            thread_idx++;
            if (thread_idx == threads) {
                thread_idx = 0;
            }

        } while (increment_bits(input));
    }

    worker_pool.wait();
    worker_pool.stop();
    sem_destroy(&queue_semaphore);
    for (int i = 0; i < threads; i++) {
        pthread_mutex_destroy(&cipher_locks[i]);
    }

    return 0;
}
