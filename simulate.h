/*
 * simulate.h -- production model for SteelSim.
 *
 * CSCI291 Programming for Engineers, Assignment 1.
 * Student: Wu Xinhao (ZZU 202587010421 / UOW 1270965)
 *
 * One item is produced by two calls: generate_item() decides what the press
 * makes, check_defect() inspects it.  The split keeps the two questions
 * ("what did we make?" and "is it any good?") in separate functions, which is
 * also why the defect field is not filled in by the factory call.
 */

#ifndef STEELSIM_SIMULATE_H
#define STEELSIM_SIMULATE_H

#include "profile.h"

/* Largest order the command-line parser accepts (mirrors cli.c). */
#define MAX_ITEMS 300

/* Product families made on the line. */
typedef enum {
    ROD = 0,
    SHEET = 1
} ProductType;

/* Failure modes.  The values double as bit flags: bit 0 is the dimensional
   test, bit 1 is the random process fault, so both failing gives 3. */
typedef enum {
    NO_DEFECT = 0,
    DIMENSIONAL_DEFECT = 1,
    RANDOM_DEFECT = 2,
    BOTH_DEFECTS = 3
} DefectType;

typedef struct {
    int id;
    ProductType type;
    double spec;
    DefectType defect;
} Item;

/* Produce one item.  Draws two random numbers, in this order: product type,
   then specification.  The defect field is not assigned here at all: only
   check_defect() may write it. */
Item generate_item(int id, const StudentProfile *profile);

/* Inspect an item and record the verdict in *item.  Always draws exactly one
   random number, even when the dimensional test has already failed, so that
   the random sequence stays aligned with the reference implementation.
   Returns non-zero when the item is defective. */
int check_defect(Item *item, const StudentProfile *profile);

#endif /* STEELSIM_SIMULATE_H */
