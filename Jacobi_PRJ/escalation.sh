#!/bin/bash
for i in {1..20}; do  ./build/jacobi_threadPool 1000 100 $i 0; done
for i in {1..20}; do  ./build/jacobi_barrier 1000 100 $i 0; done
for i in {1..20}; do  ./build/jacobi_fastflow 1000 100 $i 0; done
for i in {1..20}; do  ./build/jacobi_sequential 1000 100 0; done