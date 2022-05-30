#!/bin/bash
ITERS=100
DIM=1000

for i in {1..8}; do  ./build/jacobi_threadPool $DIM $ITERS $i 0; done
for i in {1..8}; do  ./build/jacobi_barrier $DIM $ITERS $i 0; done
for i in {1..8}; do  ./build/jacobi_fastflowbar $DIM $ITERS $i 0; done
for i in {1..8}; do  ./build/jacobi_fastflowpar $DIM $ITERS $i 0; done
for i in {1..8}; do  ./build/jacobi_sequential $DIM $ITERS 0; done