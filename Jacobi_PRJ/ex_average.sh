#!/bin/bash
NW=64
NW2=128
ITERS=100
DIM=1280

for i in {1..20}; do  ./build/jacobi_std $DIM $ITERS $NW 0; done
for i in {1..20}; do  ./build/jacobi_fastflow $DIM $ITERS $NW 0; done

for i in {1..20}; do  ./build/jacobi_std $DIM $ITERS $NW2 0; done
for i in {1..20}; do  ./build/jacobi_fastflow $DIM $ITERS $NW2 0; done

for i in {1..20}; do  ./build/jacobi_sequential $DIM $ITERS 0; done