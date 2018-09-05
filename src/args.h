/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#pragma once

#include <istream>
#include <ostream>
#include <string>
#include <vector>

namespace fasttext {

enum class model_name : int { cbow = 1, sg, sup };
enum class loss_name : int { hs = 1, ns, softmax };

class Args {
  protected:
    std::string lossToString(loss_name) const;
    std::string boolToString(bool) const;
    std::string modelToString(model_name) const;

  public:
    Args();
    std::string input;
    std::string output;
    double lr;
    int lrUpdateRate;
    int dim;
    int ws;
    int epoch;
    int minCount;
    int minCountLabel;
    int neg;
    int wordNgrams;
    loss_name loss;
    model_name model;
    int bucket;
    int minn;  // [neo] min subword (eumjeol) n-gram size
    int maxn;  // [neo] max subword (eumjeol) n-gram size
    int minnJamo;  // [neo] min subword jamo-level n-gram size
    int maxnJamo;  // [neo] max subword jamo-level n-gram size
    int minnCompact; // [neo] min compact-line subword (eumjeol) n-gram size
    int maxnCompact; // [neo] max compact-line subword (eumjeol) n-gram size
    int thread;
    double t;
    std::string label;
    int verbose;
    std::string pretrainedVectors;
    bool jamoLevel;  // [neo] a flag to tell if running in jamo level
    bool sylLevel;  // [neo] a flag to tell if running in syllable level
    bool compactLevel;  // [neo] a flag to tell if running in 'space error robust' mode
    bool saveOutput;

    bool qout;
    bool retrain;
    bool qnorm;
    size_t cutoff;
    size_t dsub;

    void parseArgs(const std::vector<std::string>& args);
    void printHelp();
    void printBasicHelp();
    void printDictionaryHelp();
    void printTrainingHelp();
    void printQuantizationHelp();
    void save(std::ostream&);
    void load(std::istream&);
    void dump(std::ostream&) const;
};
}
