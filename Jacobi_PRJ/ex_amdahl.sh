#!/bin/bash
NW=128
DIM=640
ITERS=100

for (( NW=1; NW<=128; NW++ )); do
    ./build/jacobi_std $DIM $ITERS $NW 0
done

for (( NW=1; NW<=128; NW++ )); do
    ./build/jacobi_fastflow $DIM $ITERS $NW 0
done

for (( NW=1; NW<=128; NW++ )); do
    ./build/jacobi_sequential $DIM $ITERS 0
done
