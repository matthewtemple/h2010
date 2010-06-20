#include "h/external/external.h"
#include "h/math/stats.h"

/*
 * NOTE: Regarding Recall, Precision and F-Measure:
 *
 * TP = true positive
 * FP = false positive
 * FN = false negative
 *
 * Precision:
 *   - probability that a random classification of object is correct;
 *     e.g. the probability that if i declare a mushroom poisonous,
 *     it is poisonous.
 *
 * Recall:
 *   - probability that a random object is correctly classified;
 *     e.g. the probability that if i pick a random poisonous mushroom,
 *     i will correctly classify it as poisonous.
 *
 * Precision: p = (TP / (TP + FP))
 * Recall   : r = (TP / (TP + FN))
 *
 * F1-Measure:  2pr / (p + r)       [if value precision and recall equally]
 * F2-Measure:  5pr / (4p + r)      [if value recall twice as much as precision]
 * F.5-Measure: 1.25pr / (.25p + r) [if value recall half as much as precision]
 *
 * General form: Fb-Measure: (1 + b^2) * pr / ((b^2)*p + r)
 *
 */
double h_math_stats_get_precision(unsigned long true_positives,
        unsigned long false_positives)
{
    assert(true_positives >= 0);
    assert(false_positives >= 0);

    double denominator = 0.0;

    denominator = (double)(true_positives + false_positives);
    if (denominator ==  0.0) {
        return 0.0;
    }

    return (true_positives / denominator);
}

double h_math_stats_get_recall(unsigned long true_positives,
        unsigned long false_negatives)
{
    assert(true_positives >= 0);
    assert(false_negatives >= 0);

    double denominator = 0.0;

    denominator = (double)(true_positives + false_negatives);
    if (denominator == 0.0) {
        return 0.0;
    }

    return (true_positives / denominator);
}

double h_math_stats_get_f_measure(double precision, double recall,
        double recall_weight)
{
    assert(precision >= 0.0);
    assert(recall >= 0.0);
    assert(recall_weight >= 0.0);

    double denominator = 0.0;
    double weight_squared = 0.0;

    weight_squared = pow(recall_weight, 2);
    denominator = ((weight_squared * precision) + recall);
    if (denominator == 0.0) {
        return 0.0;
    }

    return (((1 + weight_squared) * (precision * recall)) / denominator);
}

void h_math_stats_get_prf(unsigned long true_positives,
        unsigned long false_positives, unsigned long false_negatives,
        double recall_weight, double *precision, double *recall,
        double *f_measure)
{
    *precision = h_math_stats_get_precision(true_positives, false_positives);
    *recall = h_math_stats_get_recall(true_positives, false_negatives);
    *f_measure = h_math_stats_get_f_measure(*precision, *recall, recall_weight);
}
