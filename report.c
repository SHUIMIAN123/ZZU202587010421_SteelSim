#include <stdio.h>

#include "report.h"

/*
 * The field names, their order and the number formats are fixed by the brief,
 * so the whole report is assembled in memory first and written with a single
 * fwrite().  That keeps the output all-or-nothing: either the file is complete
 * or it is not created properly, which is easier to spot than a half-written
 * report.
 */

#define REPORT_BUFFER 1024

int write_report(const char *path,
                 const char *student_id,
                 int num_items,
                 const StudentProfile *profile,
                 double mean_spec,
                 double stddev_spec,
                 const ReportSummary *summary)
{
    char text[REPORT_BUFFER];
    int used = 0;
    int room = REPORT_BUFFER;
    double defect_rate;
    size_t written;
    FILE *fp;

    if (path == NULL || profile == NULL || summary == NULL) {
        return -1;
    }

    defect_rate = (num_items > 0)
                      ? (double)summary->total_defects / (double)num_items
                      : 0.0;

    /* snprintf() returns what it would have written, so clamp 'room' each
       time and let the final length be checked against the buffer. */
#define APPEND(...)                                                    \
    do {                                                               \
        int n = snprintf(text + used, (size_t)room, __VA_ARGS__);      \
        if (n < 0 || n >= room) {                                      \
            return -1;                                                 \
        }                                                              \
        used += n;                                                     \
        room -= n;                                                     \
    } while (0)

    APPEND("STUDENT_ID: %s\n", student_id);
    APPEND("NUM_ITEMS: %d\n", num_items);
    APPEND("SEED: %u\n", profile->seed);
    APPEND("DEFECT_PROB: %.4f\n", profile->defect_prob);
    APPEND("TOLERANCE: %.4f\n", profile->tolerance);
    APPEND("MIN_SPEC: %.4f\n", profile->min_spec);
    APPEND("MAX_SPEC: %.4f\n", profile->max_spec);
    APPEND("TYPE_RATIO: %.4f\n", profile->type_ratio);
    APPEND("\n");
    APPEND("TOTAL_DEFECTS: %d\n", summary->total_defects);
    APPEND("DEFECT_RATE: %.4f\n", defect_rate);
    APPEND("MEAN_SPEC: %.4f\n", mean_spec);
    APPEND("STDDEV_SPEC: %.4f\n", stddev_spec);
    APPEND("\n");
    APPEND("COUNT_ROD: %d\n", summary->count[ROD]);
    APPEND("COUNT_SHEET: %d\n", summary->count[SHEET]);
    APPEND("DEFECT_ROD: %d\n", summary->defect[ROD]);
    APPEND("DEFECT_SHEET: %d\n", summary->defect[SHEET]);

#undef APPEND

    fp = fopen(path, "wb");
    if (fp == NULL) {
        return -1;
    }

    written = fwrite(text, 1u, (size_t)used, fp);

    if (written != (size_t)used || fclose(fp) != 0) {
        return -1;
    }

    return 0;
}

