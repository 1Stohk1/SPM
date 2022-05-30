#!/bin/bash
NW=8
ITERS=100
DIM=1000

for i in {1..20}; do  ./build/jacobi_std $DIM $ITERS $NW 0; done
for i in {1..20}; do  ./build/jacobi_fastflow $DIM $ITERS $NW 0; done
for i in {1..20}; do  ./build/jacobi_sequential $DIM $ITERS 0; done