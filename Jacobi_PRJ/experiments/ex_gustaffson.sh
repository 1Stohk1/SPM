
#!/bin/bash
NW=128
MUL=20
ITERS=100

for (( NW=1; NW<=128; NW++ )); do
    ../build/jacobi_std $((MUL * NW)) $ITERS $NW 0
done

for (( NW=1; NW<=128; NW++ )); do
    ../build/jacobi_fastflow $((MUL * NW)) $ITERS $NW 0
done

for (( NW=1; NW<=128; NW++ )); do
    ../build/jacobi_sequential $((MUL * NW)) $ITERS 0
done
