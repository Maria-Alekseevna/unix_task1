#!/bin/bash

set -e

echo "=== BUILD ===" > result.txt
make clean >> result.txt 2>&1
make >> result.txt 2>&1

echo -e "\n=== TEST 1: Create file A ===" >> result.txt
./create_test_A.sh >> result.txt 2>&1

echo -e "\n=== TEST 2: Create sparse B ===" >> result.txt
./myprogram A B >> result.txt 2>&1

echo -e "\n=== TEST 3: gzip A and B ===" >> result.txt
gzip -c A > A.gz
gzip -c B > B.gz

echo -e "\n=== TEST 4: Restore B -> C ===" >> result.txt
gzip -cd B.gz | ./myprogram C >> result.txt 2>&1

echo -e "\n=== TEST 5: Block size 100 -> D ===" >> result.txt
./myprogram -b 100 A D >> result.txt 2>&1

echo -e "\n=== TEST 6: stat sizes ===" >> result.txt

for f in A A.gz B B.gz C D; do
    echo "--- $f ---" >> result.txt
    stat --format="Size: %s, Blocks: %b" $f >> result.txt
done

echo -e "\n=== DONE ===" >> result.txt

echo "Tests completed. See result.txt"