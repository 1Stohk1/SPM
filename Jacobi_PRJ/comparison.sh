#!/bin/bash
for i in jacobi_async jacobi_barrier jacobi_threadPool jacobi_fastflow; do  ./build/$i 1000 100 10 0; done
./build/jacobi_sequential 1000 100 0
