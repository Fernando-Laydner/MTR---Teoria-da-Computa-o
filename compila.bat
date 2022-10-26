gcc -Wno-discarded-qualifiers -Wpedantic -Wall -Wextra -o mtr mtr.c
type  entrada-quintupla.txt | mtr && echo success
