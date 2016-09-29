#!/bin/bash

gcc -o qe-solver -std=c99 qe-solver.c main.c -pedantic -Wall -lm
gcc -o qe-solver-unit-tests -std=c99 qe-solver.c qe-unit-tests.c -pedantic -Wall -lm
