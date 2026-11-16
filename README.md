# CSCI291 Assignment 1 — SteelSim (Personalised Production Simulation)

Student: Wu Xinhao (吴辛豪) · ZZU 202587010421 · UOW 1270965

A defect-simulation pipeline for a steel-rolling plant. The program parses the
command line, derives a per-student profile from the student id, seeds the
supplied deterministic RNG, simulates N items, classifies each as ROD / SHEET and
defective / sound, computes descriptive statistics, and writes the strictly
formatted report described in the assignment brief.

## Files

| File | Contents |
|---|---|
| main.c | command line, profile, driver, console summary |
| simulate.c/.h | product type, item generation, defect classification |
| stats.c/.h | mean and sample standard deviation (pointer traversal) |
| eport.c/.h | the sixteen-field report writer |
| Makefile | build rules |
| eport.txt | output of the run documented in the report |

cli.c, profile.c and ng.c are supplied by the unit and are **not** part of
this submission.

## Build and run

`sh
gcc -Wall -Wextra -std=c11 main.c simulate.c stats.c report.c \
    cli.c profile.c rng.c -o steelsim -lm
./steelsim -id 1270965 -n 100 -o report.txt
`

Expected summary for the submission run: 89 defects (0.8900), ROD 30/38,
SHEET 59/62, mean spec 15.8296, sample stddev 3.7156.
