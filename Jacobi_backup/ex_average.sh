#!/bin/bash
NW=8
ITERS=100
DIM=1000

for i in {1..20}; do  ./build/jacobi_threadPool $DIM $ITERS $NW 0; done
for i in {1..20}; do  ./build/jacobi_barrier $DIM $ITERS $NW 0; done
for i in {1..20}; do  ./build/jacobi_fastflowbar $DIM $ITERS $NW 0; done
for i in {1..20}; do  ./build/jacobi_fastflowpar $DIM $ITERS $NW 0; done
for i in {1..20}; do  ./build/jacobi_sequential $DIM $ITERS 0; done