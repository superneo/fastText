#!/usr/bin/env bash

if [ $# -lt 3 ]; then
  echo "Usgae: $0 mode(word/syllable/jamo) word_ngram_size(1,2,3,..) [subword_ngram_min_size(3 by default)] [subword_ngram_max_size(6 by default)]"
  exit 1
fi

MODE=$1
WNGRAM=$2
MINN=3  # minimum subword ngram as 3 by default
MAXN=6  # maximum subword ngram as 6 by default

if [ $# -eq 5 ]; then
  MINN=$3
  MAXN=$4
fi

PRETRAINING_CORPUS="korean_go_kr_sortuniqed.txt"
TRAINING_DATASET="nsmc_train.txt"
TEST_DATASET="nsmc_test.txt"


# skipgram learning
echo "[skipgram] (${PRETRAINING_CORPUS}) models/${MODE}/sg_${MODE}_w${WNGRAM}gram"
../fasttext skipgram -input data/${PRETRAINING_CORPUS} -output models/${MODE}/sg_${MODE}_w${WNGRAM}gram -lr 0.025 -dim 100 -ws 5 -epoch 10 -minCount 5 -neg 5 -loss ns -bucket 2000000 -wordNgrams ${WNGRAM} -minn ${MINN} -maxn ${MAXN} -thread 10 -t 1e-4 -lrUpdateRate 100

# supervised learning with training dataset only
echo "[supervised] (${TRAINING_DATASET}) models/${MODE}/sup_trainData_only_${MODE}_w${WNGRAM}gram"
../fasttext supervised -input data/${TRAINING_DATASET} -output models/${MODE}/sup_trainData_only_${MODE}_w${WNGRAM}gram -lr 0.025 -dim 100 -ws 5 -epoch 10 -minCount 5 -neg 5 -loss ns -bucket 2000000 -wordNgrams ${WNGRAM} -minn ${MINN} -maxn ${MAXN} -thread 10 -t 1e-4 -lrUpdateRate 100

# prediction with probabilities by training dataset only
echo "[preciction with prob] (models/${MODE}/sup_trainData_only_${MODE}_w${WNGRAM}gram.bin data/${TEST_DATASET}) preds/${MODE}/pred_trainData_only_${MODE}_w${WNGRAM}gram.predict"
../fasttext predict-prob models/${MODE}/sup_trainData_only_${MODE}_w${WNGRAM}gram.bin data/${TEST_DATASET} > preds/${MODE}/pred_trainData_only_${MODE}_w${WNGRAM}gram.predict

# evaluation for prediction by training dataset only
echo "[evaluation] (data/${TEST_DATASET} vs preds/${MODE}/pred_trainData_only_${MODE}_w${WNGRAM}gram.predict) evals/${MODE}/eval_trainData_only_${MODE}_w${WNGRAM}gram.txt"
./eval_predictions.py data/${TEST_DATASET} preds/${MODE}/pred_trainData_only_${MODE}_w${WNGRAM}gram.predict evals/${MODE}/eval_trainData_only_${MODE}_w${WNGRAM}gram.txt


# supervised learning with training dataset + pretrained wordvec
echo "[supervised] (${TRAINING_DATASET} + models/${MODE}/sg_${MODE}_w${WNGRAM}gram.vec) models/${MODE}/sup_trainData_wordvec_${MODE}_w${WNGRAM}gram"
../fasttext supervised -input data/${TRAINING_DATASET} -output models/${MODE}/sup_trainData_wordvec_${MODE}_w${WNGRAM}gram -pretrainedVectors models/${MODE}/sg_${MODE}_w${WNGRAM}gram.vec -lr 0.025 -dim 100 -ws 5 -epoch 10 -minCount 5 -neg 5 -loss ns -bucket 2000000 -wordNgrams ${WNGRAM} -minn ${MINN} -maxn ${MAXN} -thread 10 -t 1e-4 -lrUpdateRate 100

# prediction with probabilities by training dataset + pretrained wordvec
echo "[preciction with prob] (models/${MODE}/sup_trainData_wordvec_${MODE}_w${WNGRAM}gram.bin data/${TEST_DATASET}) preds/${MODE}/pred_trainData_wordvec_${MODE}_w${WNGRAM}gram.predict"
../fasttext predict-prob models/${MODE}/sup_trainData_wordvec_${MODE}_w${WNGRAM}gram.bin data/${TEST_DATASET} > preds/${MODE}/pred_trainData_wordvec_${MODE}_w${WNGRAM}gram.predict

# evaluation for prediction by training dataset + pretrained wordvec
echo "[evaluation] (data/${TEST_DATASET} vs preds/${MODE}/pred_trainData_wordvec_${MODE}_w${WNGRAM}gram.predict) evals/${MODE}/eval_trainData_wordvec_${MODE}_w${WNGRAM}gram.txt"
./eval_predictions.py data/${TEST_DATASET} preds/${MODE}/pred_trainData_wordvec_${MODE}_w${WNGRAM}gram.predict evals/${MODE}/eval_trainData_wordvec_${MODE}_w${WNGRAM}gram.txt
