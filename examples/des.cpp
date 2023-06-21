#include "builders/cipheranalyzerbuilder.h"
#include "src/box/sbox.h"
#include "src/box/xorbox.h"
#include "src/cipheranalyzer.h"
#include "src/helpers/helpers.h"
#include "src/roundfunction.h"
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/bind.hpp>
#include <boost/dynamic_bitset.hpp>
#include <builders/eboxbuilder.h>
#include <builders/identityboxbuilder.h>
#include <builders/pboxbuilder.h>
#include <builders/sboxbuilder.h>
#include <builders/xorboxbuilder.h>
#include <cstdio>
#include <examples/des_boxes.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;
using namespace boost;

const int max_weight = 6;
const int max_inputs = 16;
const int round_cnt = 5;
const int threads = 24;
const int max_queue_size = 1 << 20;

CipherAnalyzerBuilderPtr create_cipher_builder() {
    AbstractBoxBuilderPtr ebox_builder = make_ebox_builder(bit_expansion);
    AbstractBoxBuilderPtr pbox_builder = make_pbox_builder(bit_perm);

    AbstractBoxBuilderPtr sbox_builder_1 = make_sbox_builder(6, 4, compute_diff_dist_table(sbox1), true);
    AbstractBoxBuilderPtr sbox_builder_2 = make_sbox_builder(6, 4, compute_diff_dist_table(sbox2), true);
    AbstractBoxBuilderPtr sbox_builder_3 = make_sbox_builder(6, 4, compute_diff_dist_table(sbox3), true);
    AbstractBoxBuilderPtr sbox_builder_4 = make_sbox_builder(6, 4, compute_diff_dist_table(sbox4), true);
    AbstractBoxBuilderPtr sbox_builder_5 = make_sbox_builder(6, 4, compute_diff_dist_table(sbox5), true);
    AbstractBoxBuilderPtr sbox_builder_6 = make_sbox_builder(6, 4, compute_diff_dist_table(sbox6), true);
    AbstractBoxBuilderPtr sbox_builder_7 = make_sbox_builder(6, 4, compute_diff_dist_table(sbox7), true);
    AbstractBoxBuilderPtr sbox_builder_8 = make_sbox_builder(6, 4, compute_diff_dist_table(sbox8), true);

    AbstractBoxBuilderPtr sbox_builder_1_non_exhaust = make_sbox_builder(6, 4, compute_diff_dist_table(sbox1), true);
    AbstractBoxBuilderPtr sbox_builder_2_non_exhaust = make_sbox_builder(6, 4, compute_diff_dist_table(sbox2), true);
    AbstractBoxBuilderPtr sbox_builder_3_non_exhaust = make_sbox_builder(6, 4, compute_diff_dist_table(sbox3), true);
    AbstractBoxBuilderPtr sbox_builder_4_non_exhaust = make_sbox_builder(6, 4, compute_diff_dist_table(sbox4), true);
    AbstractBoxBuilderPtr sbox_builder_5_non_exhaust = make_sbox_builder(6, 4, compute_diff_dist_table(sbox5), true);
    AbstractBoxBuilderPtr sbox_builder_6_non_exhaust = make_sbox_builder(6, 4, compute_diff_dist_table(sbox6), true);
    AbstractBoxBuilderPtr sbox_builder_7_non_exhaust = make_sbox_builder(6, 4, compute_diff_dist_table(sbox7), true);
    AbstractBoxBuilderPtr sbox_builder_8_non_exhaust = make_sbox_builder(6, 4, compute_diff_dist_table(sbox8), true);

    AbstractBoxBuilderPtr xorbox_builder = make_xorbox_builder(32);
    AbstractBoxBuilderPtr identitybox_builder = make_identitybox_builder(64);

    RoundBuilderPtr inner_round_builder = make_round_builder();
    RoundBuilderPtr outer_round_builder = make_round_builder();

    outer_round_builder->add_builder("E", ebox_builder);
    outer_round_builder->add_builder("P", pbox_builder);
    outer_round_builder->add_builder("S1_non_exhaust", sbox_builder_1_non_exhaust);
    outer_round_builder->add_builder("S2_non_exhaust", sbox_builder_2_non_exhaust);
    outer_round_builder->add_builder("S3_non_exhaust", sbox_builder_3_non_exhaust);
    outer_round_builder->add_builder("S4_non_exhaust", sbox_builder_4_non_exhaust);
    outer_round_builder->add_builder("S5_non_exhaust", sbox_builder_5_non_exhaust);
    outer_round_builder->add_builder("S6_non_exhaust", sbox_builder_6_non_exhaust);
    outer_round_builder->add_builder("S7_non_exhaust", sbox_builder_7_non_exhaust);
    outer_round_builder->add_builder("S8_non_exhaust", sbox_builder_8_non_exhaust);
    outer_round_builder->add_builder("Xor", xorbox_builder);
    outer_round_builder->add_builder("Src", identitybox_builder);
    outer_round_builder->add_builder("Dst", identitybox_builder);

    outer_round_builder->add_connection("Src", "E", BitsRange(0, 32), BitsRange(0, 32));
    outer_round_builder->add_connection("Src", "Xor", BitsRange(32, 32), BitsRange(0, 32));
    outer_round_builder->add_connection("Src", "Dst", BitsRange(0, 32), BitsRange(32, 32));

    outer_round_builder->add_connection("E", "S8_non_exhaust", BitsRange(0, 6), BitsRange(0, 6));
    outer_round_builder->add_connection("E", "S7_non_exhaust", BitsRange(6, 6), BitsRange(0, 6));
    outer_round_builder->add_connection("E", "S6_non_exhaust", BitsRange(12, 6), BitsRange(0, 6));
    outer_round_builder->add_connection("E", "S5_non_exhaust", BitsRange(18, 6), BitsRange(0, 6));
    outer_round_builder->add_connection("E", "S4_non_exhaust", BitsRange(24, 6), BitsRange(0, 6));
    outer_round_builder->add_connection("E", "S3_non_exhaust", BitsRange(30, 6), BitsRange(0, 6));
    outer_round_builder->add_connection("E", "S2_non_exhaust", BitsRange(36, 6), BitsRange(0, 6));
    outer_round_builder->add_connection("E", "S1_non_exhaust", BitsRange(42, 6), BitsRange(0, 6));

    outer_round_builder->add_connection("S8_non_exhaust", "P", BitsRange(0, 4), BitsRange(0, 4));
    outer_round_builder->add_connection("S7_non_exhaust", "P", BitsRange(0, 4), BitsRange(4, 4));
    outer_round_builder->add_connection("S6_non_exhaust", "P", BitsRange(0, 4), BitsRange(8, 4));
    outer_round_builder->add_connection("S5_non_exhaust", "P", BitsRange(0, 4), BitsRange(12, 4));
    outer_round_builder->add_connection("S4_non_exhaust", "P", BitsRange(0, 4), BitsRange(16, 4));
    outer_round_builder->add_connection("S3_non_exhaust", "P", BitsRange(0, 4), BitsRange(20, 4));
    outer_round_builder->add_connection("S2_non_exhaust", "P", BitsRange(0, 4), BitsRange(24, 4));
    outer_round_builder->add_connection("S1_non_exhaust", "P", BitsRange(0, 4), BitsRange(28, 4));

    outer_round_builder->add_connection("P", "Xor", BitsRange(0, 32), BitsRange(32, 32));
    outer_round_builder->add_connection("Xor", "Dst", BitsRange(0, 32), BitsRange(0, 32));

    outer_round_builder->set_src("Src");
    outer_round_builder->set_dst("Dst");

    inner_round_builder->add_builder("E", ebox_builder);
    inner_round_builder->add_builder("P", pbox_builder);
    inner_round_builder->add_builder("S1", sbox_builder_1);
    inner_round_builder->add_builder("S2", sbox_builder_2);
    inner_round_builder->add_builder("S3", sbox_builder_3);
    inner_round_builder->add_builder("S4", sbox_builder_4);
    inner_round_builder->add_builder("S5", sbox_builder_5);
    inner_round_builder->add_builder("S6", sbox_builder_6);
    inner_round_builder->add_builder("S7", sbox_builder_7);
    inner_round_builder->add_builder("S8", sbox_builder_8);
    inner_round_builder->add_builder("Xor", xorbox_builder);
    inner_round_builder->add_builder("Src", identitybox_builder);
    inner_round_builder->add_builder("Dst", identitybox_builder);

    inner_round_builder->add_connection("Src", "E", BitsRange(0, 32), BitsRange(0, 32));
    inner_round_builder->add_connection("Src", "Xor", BitsRange(32, 32), BitsRange(0, 32));
    inner_round_builder->add_connection("Src", "Dst", BitsRange(0, 32), BitsRange(32, 32));

    inner_round_builder->add_connection("E", "S8", BitsRange(0, 6), BitsRange(0, 6));
    inner_round_builder->add_connection("E", "S7", BitsRange(6, 6), BitsRange(0, 6));
    inner_round_builder->add_connection("E", "S6", BitsRange(12, 6), BitsRange(0, 6));
    inner_round_builder->add_connection("E", "S5", BitsRange(18, 6), BitsRange(0, 6));
    inner_round_builder->add_connection("E", "S4", BitsRange(24, 6), BitsRange(0, 6));
    inner_round_builder->add_connection("E", "S3", BitsRange(30, 6), BitsRange(0, 6));
    inner_round_builder->add_connection("E", "S2", BitsRange(36, 6), BitsRange(0, 6));
    inner_round_builder->add_connection("E", "S1", BitsRange(42, 6), BitsRange(0, 6));

    inner_round_builder->add_connection("S8", "P", BitsRange(0, 4), BitsRange(0, 4));
    inner_round_builder->add_connection("S7", "P", BitsRange(0, 4), BitsRange(4, 4));
    inner_round_builder->add_connection("S6", "P", BitsRange(0, 4), BitsRange(8, 4));
    inner_round_builder->add_connection("S5", "P", BitsRange(0, 4), BitsRange(12, 4));
    inner_round_builder->add_connection("S4", "P", BitsRange(0, 4), BitsRange(16, 4));
    inner_round_builder->add_connection("S3", "P", BitsRange(0, 4), BitsRange(20, 4));
    inner_round_builder->add_connection("S2", "P", BitsRange(0, 4), BitsRange(24, 4));
    inner_round_builder->add_connection("S1", "P", BitsRange(0, 4), BitsRange(28, 4));

    inner_round_builder->add_connection("P", "Xor", BitsRange(0, 32), BitsRange(32, 32));
    inner_round_builder->add_connection("Xor", "Dst", BitsRange(0, 32), BitsRange(0, 32));

    inner_round_builder->set_src("Src");
    inner_round_builder->set_dst("Dst");

    CipherAnalyzerBuilderPtr cipher_builder = make_cipher_analyzer_builder();
    cipher_builder->add_round_builder("OuterRound", outer_round_builder);
    cipher_builder->add_round_builder("InnerRound", inner_round_builder);

    cipher_builder->add_round_name("OuterRound");
    for (int i = 0; i < round_cnt - 2; i++) {
        cipher_builder->add_round_name("InnerRound");
    }
    if (round_cnt > 2) {
        cipher_builder->add_round_name("OuterRound");
    }

    return cipher_builder;
}

void job(CipherAnalyzerBuilderPtr &cipher_builder, CipherAnalyzerPtr &cipher, pthread_mutex_t *cipher_lock,
         dynamic_bitset<> &input, int thread_idx, sem_t *queue_semaphore) {
    pthread_mutex_lock(cipher_lock);

    cipher->set_input(input, BitsRange(0, cipher->get_input_size()));
    auto [output, prob] = cipher->get_next_state();

    while (output.size() > 0) {
        if (prob > 0) {
            printf("[%02d] in: 0x%s (%02ld),\tout: 0x%s (%02ld) -> prob %.30lf\n", thread_idx,
                   convert_to_hex_string(input).c_str(), input.count(), convert_to_hex_string(output).c_str(),
                   output.count(), prob);
        }

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

    // 1.0 preffix and first round of a feistel cipher has a trivial differential trail of probability 1.0
    cipher_builder->set_opt_probs(vector<double>{1.0, 1.0});

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
