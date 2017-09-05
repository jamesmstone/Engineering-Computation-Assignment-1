#!/bin/sh
set -e

gcc -Wall -ansi -o code assmt1.c -lm && ./code < tests/input/1.txt  > out.txt && diff out.txt tests/output/1.txt && rm out.txt code && gcc -Wall -ansi -o code assmt1.c -lm && ./code < tests/input/0.txt  > out.txt && diff out.txt tests/output/0.txt && rm out.txt code
