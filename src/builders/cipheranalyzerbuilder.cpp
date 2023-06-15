#include "cipheranalyzerbuilder.h"

void CipherAnalyzerBuilder::notify_ciphers() {
    for (auto &cipher : built_ciphers) {
        cipher->update_global_thresh(global_thresh);
    }
}

CipherAnalyzerBuilder::CipherAnalyzerBuilder() : global_thresh(0.0) { pthread_mutex_init(&global_thresh_lock, NULL); }

CipherAnalyzerBuilder::~CipherAnalyzerBuilder() { pthread_mutex_destroy(&global_thresh_lock); }

void CipherAnalyzerBuilder::add_round_builder(string name, RoundBuilderPtr round_builder) {
    round_builders[name] = round_builder;
}

void CipherAnalyzerBuilder::add_round_name(string name) { round_names.push_back(name); }

void CipherAnalyzerBuilder::set_opt_probs(vector<double> opt_probs) { this->opt_probs = opt_probs; }

void CipherAnalyzerBuilder::set_global_thresh(double global_thresh) { this->global_thresh = global_thresh; }

CipherAnalyzerPtr CipherAnalyzerBuilder::build() {
    pthread_mutex_lock(&global_thresh_lock);
    if (!opt_probs.empty()) {
        CipherAnalyzerPtr new_cipher = make_cipher_analyzer(round_builders, round_names, global_thresh, opt_probs);
        built_ciphers.push_back(new_cipher);
        pthread_mutex_unlock(&global_thresh_lock);
        return new_cipher;
    }

    CipherAnalyzerPtr new_cipher = make_cipher_analyzer(round_builders, round_names, global_thresh);

    built_ciphers.push_back(new_cipher);
    pthread_mutex_unlock(&global_thresh_lock);

    return new_cipher;
}

void CipherAnalyzerBuilder::update_global_thresh(double global_thresh) {
    pthread_mutex_lock(&global_thresh_lock);
    this->global_thresh = global_thresh;
    notify_ciphers();
    pthread_mutex_unlock(&global_thresh_lock);
}

CipherAnalyzerBuilderPtr make_cipher_analyzer_builder() { return std::make_shared<CipherAnalyzerBuilder>(); }
