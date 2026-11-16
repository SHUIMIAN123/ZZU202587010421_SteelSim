#include <math.h>

#include "rng.h"
#include "simulate.h"

/*
 * The random-number order is fixed by the assignment:
 *
 *   draw 1 -> product type      (generate_item)
 *   draw 2 -> specification     (generate_item)
 *   draw 3 -> random process    (check_defect, taken unconditionally)
 *
 * Three draws per item, always.  The defect classification is then a small
 * bit-mask exercise: set bit 0 for a dimensional failure, bit 1 for a random
 * one, and let the enum values (0, 1, 2, 3) do the mapping.
 */

#define FLAG_DIMENSIONAL 0x1u
#define FLAG_RANDOM 0x2u

/* Next value from the unit stream, kept in one place so the call count is
   easy to audit. */
static double next_draw(void)
{
    return rng_uniform01();
}

/* A rod is made when the draw falls below the rod share of the order. */
static ProductType pick_type(double rod_ratio)
{
    return (next_draw() < rod_ratio) ? ROD : SHEET;
}

/* Specifications are spread evenly over the acceptance window. */
static double pick_spec(const StudentProfile *profile)
{
    double span = profile->max_spec - profile->min_spec;

    return profile->min_spec + span * next_draw();
}

/* The plant is set up to hit whole millimetres; anything further than the
   tolerance away from the nearest whole value is out of spec. */
static int outside_tolerance(double spec, double tolerance)
{
    return fabs(spec - floor(spec + 0.5)) > tolerance;
}

Item generate_item(int id, const StudentProfile *profile)
{
    Item item;

    item.id = id;
    item.type = pick_type(profile->type_ratio);
    item.spec = pick_spec(profile);
    /* The brief forbids the factory from touching item.defect: check_defect()
       is the only writer, so the classification cannot be pre-empted here. */

    return item;
}

int check_defect(Item *item, const StudentProfile *profile)
{
    unsigned flags = 0u;

    if (outside_tolerance(item->spec, profile->tolerance)) {
        flags |= FLAG_DIMENSIONAL;
    }

    if (next_draw() < profile->defect_prob) {
        flags |= FLAG_RANDOM;
    }

    item->defect = (DefectType)flags;

    return (flags != 0u) ? 1 : 0;
}
