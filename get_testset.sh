#!/bin/bash

set -euo "pipefail"

wget -q http://steinlib.zib.de/download/B.tgz

tar -xf B.tgz

rm B.tgz