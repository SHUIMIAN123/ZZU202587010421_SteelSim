/*
 * main.c -- SteelSim: personalised production simulation.
 *
 * CSCI291 Programming for Engineers, Assignment 1.
 * Student: Wu Xinhao (ZZU 202587010421 / UOW 1270965)
 *
 * Pipeline: parse the command line, build the student profile, seed the
 * generator once, simulate the order, then write the assessed report.
 * The simulation itself lives in run_simulation() so that main() reads as a
 * list of the five steps in the brief.
 */

#include <stdio.h>

#include "cli.h"
#include "profile.h"
#include "report.h"
#include "rng.h"
#include "simulate.h"
#include "stats.h"

/* One produced item together with the specification value it contributed to
   the statistics sample. */
typedef struct {
    Item item;
    double spec;
} Record;

/* Fold one inspected item into the counters.  Indexing by ProductType keeps
   the rod and sheet branches from drifting apart. */
static void tally(ReportSummary *summary, const Item *item)
{
    int family = (int)item->type;

    summary->count[family] += 1;

    if (item->defect != NO_DEFECT) {
        summary->defect[family] += 1;
        summary->total_defects += 1;
    }
}

/* Produce num_items items, classify them and compute the sample statistics. */
static void run_simulation(const CliOptions *opts,
                           const StudentProfile *profile,
                           ReportSummary *summary,
                           double *mean_spec,
                           double *stddev_spec)
{
    Record batch[MAX_ITEMS];
    double specs[MAX_ITEMS];
    int i;

    summary->total_defects = 0;
    summary->count[ROD] = 0;
    summary->count[SHEET] = 0;
    summary->defect[ROD] = 0;
    summary->defect[SHEET] = 0;

    for (i = 0; i < opts->num_items; ++i) {
        batch[i].item = generate_item(i + 1, profile);
        check_defect(&batch[i].item, profile); /* verdict written in place */
        batch[i].spec = batch[i].item.spec;
        specs[i] = batch[i].spec;

        tally(summary, &batch[i].item);
    }

    *mean_spec = compute_mean(specs, opts->num_items);
    *stddev_spec = compute_stddev(specs, opts->num_items, *mean_spec);
}

int main(int argc, char *argv[])
{
    CliOptions opts;
    StudentProfile profile;
    ReportSummary summary;
    double mean_spec = 0.0;
    double stddev_spec = 0.0;

    if (parse_args(argc, argv, &opts) != 0) {
        print_usage(argv[0]);
        return 1;
    }

    generate_profile(opts.student_id, &profile);
    rng_seed(profile.seed);

    run_simulation(&opts, &profile, &summary, &mean_spec, &stddev_spec);

    if (write_report(opts.output_file, opts.student_id, opts.num_items,
                     &profile, mean_spec, stddev_spec, &summary) != 0) {
        fprintf(stderr, "Error: cannot write report file '%s'\n",
                opts.output_file);
        return 1;
    }

    /* Console summary -- for the operator, not part of the assessed file. */
    printf("SteelSim: %d items for student %s (seed %u)\n",
           opts.num_items, opts.student_id, profile.seed);
    printf("Defects: %d (%.4f), ROD %d/%d, SHEET %d/%d\n",
           summary.total_defects,
           (double)summary.total_defects / (double)opts.num_items,
           summary.defect[ROD], summary.count[ROD],
           summary.defect[SHEET], summary.count[SHEET]);
    printf("Mean spec: %.4f, sample stddev: %.4f\n", mean_spec, stddev_spec);
    printf("Report written to %s\n", opts.output_file);

    return 0;
}
