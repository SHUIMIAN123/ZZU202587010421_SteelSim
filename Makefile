# SteelSim -- build file
# CSCI291 Assignment 1, Wu Xinhao (ZZU 202587010421 / UOW 1270965)
#
# cli.c, profile.c and rng.c are supplied by the unit and are NOT part of this
# submission.  Copy them into this directory before running make.

CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -O2
LDLIBS  := -lm
TARGET  := steelsim
OBJS    := main.o simulate.o stats.o report.o cli.o profile.o rng.o

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDLIBS)

# Every module only needs its own header plus the unit-supplied ones.
main.o:     main.c simulate.h stats.h report.h cli.h profile.h rng.h
simulate.o: simulate.c simulate.h profile.h rng.h
stats.o:    stats.c stats.h
report.o:   report.c report.h profile.h

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) -id 1270965 -n 100 -o report.txt

clean:
	rm -f $(OBJS) $(TARGET)
