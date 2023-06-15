/** @file cipheranalyzer.h
 *  @brief implementation of the CipherAnalyzer class
 */
#ifndef CIPHERANALYZER_H
#define CIPHERANALYZER_H

#include "roundfunction.h"
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/bind.hpp>
#include <builders/roundbuilder.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

class CipherAnalyzer;

/**
 * @brief shorthand for std::shared_ptr<CipherAnalyzer>
 */
typedef std::shared_ptr<CipherAnalyzer> CipherAnalyzerPtr;

/**
 * @brief a CipherAnalyzer is a collection of rounds of a cipher that are linked togheter linearly.
 */
class CipherAnalyzer {
    friend class CipherAnalyzerBuilder;

  protected:
    /**
     * @brief the minimum probability threshold such that the cipher outputs a state.
     */
    double global_thresh;

    /**
     * @brief pthread mutex for `global_thresh`
     */
    pthread_mutex_t global_thresh_lock;

    /**
     * @brief optimal probabilities for all previous rounds
     */
    vector<double> opt_probs;

    /**
     * @brief round_builders a `map` of named round builders that represents the builders for the rounds of the
     * cipher. Used creating preffix rounds for calculating the `opt_probs` array
     */
    map<string, RoundBuilderPtr> round_builders;

    /**
     * @brief round_names the names of the rounds of the cipher. Used creating preffix rounds for calculating the
     * `opt_probs` array
     */
    vector<string> round_names;

    /**
     * @brief a collection of all rounds that form this cipher.
     */
    vector<RoundFunctionPtr> rounds;

    /**
     * @brief the index in `rounds` of the current round in the search algorithm
     */
    size_t curr_idx;

    /**
     * @brief `vector` of all previous partial probabilities up to curr_idx. Used in calculating the final
     * probability of the cipher.
     */
    vector<double> round_probs;

    /**
     * @brief advances to the next internal state state of the cipher
     * @return false if there is no next state, otherwise returns true
     */
    bool advance_state();

    /**
     * @brief updates the `global_thresh` by keeping the best threshold between the current value and the new one, uses
     * `global_thresh_lock`, is only called from a `CipherAnalyzerBuilder`'s `update_global_thresh()`
     * @param `global_thresh` the new threshold value
     */
    void update_global_thresh(double global_thresh);

  public:
    /**
     * @param a `map` of named round constructors that represents the constructors for the rounds of the cipher
     * @param the names of the rounds of the cipher, the output of of each round is set as the input of the next one
     * @param global_thresh the minimum global threshold probability that an output state must have in order to be
     * returned
     * @param opt_probs, if known, optimal probabilities of all ciphers with less rounds than this one
     */
    CipherAnalyzer(map<string, RoundBuilderPtr> round_builders, vector<string> round_names, double global_thresh,
                   vector<double> opt_probs);

    /**
     * @param a `map` of named round constructors that represents the constructors for the rounds of the cipher
     * @param the names of the rounds of the cipher, the output of of each round is set as the input of the next one
     * @param global_thresh the minimum global threshold probability that an output state must have in order to be
     * returned
     */
    CipherAnalyzer(map<string, RoundBuilderPtr> round_builders, vector<string> round_names, double global_thresh);

    ~CipherAnalyzer();

    /**
     * @brief calculates the `opt_probs` vector for smaller rounds
     * @param input_max_hamming_weight the maximum hamming weight when searching the optimal probabilities of ciphers
     * with less rounds
     * @param max_inputs the maximum size of the return value
     *
     * @return `vector<dynamic_bitset>` representing the best input values sorted by probability
     */
    vector<dynamic_bitset<>> calculate_opt_probs(size_t input_max_hamming_weight, size_t max_inputs, size_t threads,
                                                 size_t max_queue_size);

    /**
     * @brief returns the next possible output state that has a probability greater than `global_thresh`. Also sets
     * `global_thresh` to the new best probability in order to minimize the size of the search tree
     * @return the next computed output state, otherwise `{dynamic_bitset<>(0), 0.0}`
     */
    ProbEntry get_next_state();

    /**
     * @brief sets a subrange `rng` of the input to the value of `bits`
     * @param bits the bits that will be put in the `in_bits` of `src` of the first round
     * @param rng the subrange in which bits will be put in `in_bits` of `src` of the first round
     */
    void set_input(const dynamic_bitset<> &bits, BitsRange rng);

    /**
     * @brief getter for the input size of the cipher
     * @return rounds[0]->src->input_size()
     */
    size_t get_input_size();

    /**
     * @brief getter for `opt_probs`
     * @return `opt_probs`
     */
    vector<double> get_opt_probs();
};

/**
 * @brief make_cipher_analyzer
 * @param round_builders the RoundBuilders which the cipher uses to build it's internal structure
 * @param round_names the name of the rounds of which the chipher is composed
 * @param global_threshold the initial global theshold
 * @return a new CipherAnalyzerPtr with the specified arguments
 */
CipherAnalyzerPtr make_cipher_analyzer(map<string, RoundBuilderPtr> round_builders, vector<string> round_names,
                                       double global_threshold);

/**
 * @brief same as previous function, but also includes `opt_probs`
 * @param round_builders the RoundBuilders which the cipher uses to build it's internal structure
 * @param round_names the name of the rounds of which the chipher is composed
 * @param global_threshold the initial global theshold
 * @param opt_probs the initial optimal probabilites for previous rounds
 * @return a new CipherAnalyzerPtr with the specified arguments
 */
CipherAnalyzerPtr make_cipher_analyzer(map<string, RoundBuilderPtr> round_builders, vector<string> round_names,
                                       double global_threshold, vector<double> opt_probs);

#endif // CIPHERANALYZER_H
