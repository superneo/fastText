#!/usr/bin/python

import sys

def get_diff(ground_truth_file_name, prediction_file_name, diff_file_name):
    if not ground_truth_file_name or not prediction_file_name or not diff_file_name:
        print("[ERROR] invalied input argument passed.")
        sys.exit(1)

    with open(ground_truth_file_name, "rt") as inf_gt:
        gt_raw = inf_gt.read()
        gt_list = gt_raw.split('\n')[:-1]
        gt_dict = {i+1: [gt_list[i].split('\t')[0], gt_list[i].split('\t')[1]]
                   for i in range(len(gt_list))}

    with open(prediction_file_name, "rt") as inf_pr:
        pr_raw = inf_pr.read()
        pr_list = pr_raw.split('\n')[:-1]
        pr_dict = {i+1: [pr_list[i].split()[0], pr_list[i].split()[1]]
                   for i in range(len(pr_list))}

    diff_cnt = 0
    with open(diff_file_name, "wt") as outf:
        for key in sorted(pr_dict.keys()):
            if gt_dict[key][0] != pr_dict[key][0]:
                diff_cnt += 1
                outf.write(str(key) + "\t" +
                           gt_dict[key][0][-1] + "\t" + gt_dict[key][1] + "\t" +
                           pr_dict[key][0][-1] + "\t" + pr_dict[key][1] + "\n")

    print("# of total cases: " + str(len(gt_dict)))
    print("# of wrong predictions: " + str(diff_cnt))
    print("% of correct predictions: " + str(1.0 - float(diff_cnt)/len(gt_dict)))


if __name__ == "__main__":
    print("[" + __file__ + "] invoked.")

    if len(sys.argv) != 4:
        print("[Usage] python " + sys.argv[0] + " [Ground Truth Filename] [Prediction Filename] [Diff Filename]")
        sys.exit(1)

    get_diff(sys.argv[1], sys.argv[2], sys.argv[3])

