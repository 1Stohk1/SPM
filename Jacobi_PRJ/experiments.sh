#!/bin/bash
for i in jacobi_async jacobi_barrier jacobi_threadPool jacobi_sequential; do  ./build/$i 1000 100 10 123; done
