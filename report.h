/*
 * report.h -- writes the assessed report file.
 *
 * The counters are kept as two-element arrays indexed by ProductType, which
 * removes the four near-identical fields (and the copy-paste branches that go
 * with them) from main.c.
 */

#ifndef STEELSIM_REPORT_H
#define STEELSIM_REPORT_H

#include "simulate.h"   /* ProductType indexes the counters */

typedef struct {
    int total_defects; /* every defective item, either type */
    int count[2];      /* items made, indexed by ProductType  */
    int defect[2];     /* defective items, indexed by ProductType */
} ReportSummary;

/* Write the report to path: sixteen fields in three groups, as the brief's
   format section lays them out.
   Returns 0 on success, -1 if the file cannot be created or written. */
int write_report(const char *path,
                 const char *student_id,
                 int num_items,
                 const StudentProfile *profile,
                 double mean_spec,
                 double stddev_spec,
                 const ReportSummary *summary);

#endif /* STEELSIM_REPORT_H */

