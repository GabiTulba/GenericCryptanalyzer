#ifndef CIPHERANALYZERBUILDER_H
#define CIPHERANALYZERBUILDER_H

#include "cipheranalyzer.h"
#include "roundbuilder.h"
#include <map>
#include <pthread.h>
#include <string>
#include <vector>

using namespace std;

class CipherAnalyzerBuilder {
  private:
    map<string, RoundBuilderPtr> round_builders;
    vector<string> round_names;
    vector<double> opt_probs;
    double global_thresh;
    pthread_mutex_t global_thresh_lock;

    vector<CipherAnalyzerPtr> built_ciphers;

    void notify_ciphers();

  public:
    CipherAnalyzerBuilder();
    ~CipherAnalyzerBuilder();

    void add_round_builder(string name, RoundBuilderPtr round_builder);
    void add_round_name(string name);
    void set_opt_probs(vector<double> opt_probs);
    void set_global_thresh(double global_thresh);
    CipherAnalyzerPtr build();

    void update_global_thresh(double global_thresh);
};

typedef std::shared_ptr<CipherAnalyzerBuilder> CipherAnalyzerBuilderPtr;

CipherAnalyzerBuilderPtr make_cipher_analyzer_builder();

#endif // CIPHERANALYZERBUILDER_H
