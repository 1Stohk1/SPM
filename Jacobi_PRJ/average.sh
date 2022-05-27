#!/bin/bash
for i in {1..20}; do  ./build/jacobi_threadPool 1000 100 10 123; done
for i in {1..20}; do  ./build/jacobi_sequential 1000 100 10 123; done

