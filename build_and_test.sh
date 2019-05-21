#!/bin/bash

set -euo "pipefail"

cmake .
make

for f in B/*.stp
do
time ./out/steiner $f
done