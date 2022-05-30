#!/bin/bash
ITERS=100
DIM=1000

for i in {1..8}; do  ./build/jacobi_std $DIM $ITERS $i 0; done
for i in {1..8}; do  ./build/jacobi_fastflow $DIM $ITERS $i 0; done
for i in {1..8}; do  ./build/jacobi_sequential $DIM $ITERS 0; done