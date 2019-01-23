#!/bin/bash
# faces.sh
# Bash script which runs the facial recognizion program.
# -----------------------------------------------------------------------------
# Authors: Emil hallberg   (id16ehg)
#          Jonas Sjödin    (id16jsn)
# Date: October 2018
# Course: Fundamentals of artificial intelligence
# Assignment: 5
# -----------------------------------------------------------------------------


if [ $# -ne 3 ]
then
    echo "Usage: " + $0 + " <training.txt> <training_keys.txt> <test.txt>"
    exit 1
fi

python3 "$(dirname "$0")"/faces.py $1 $2 $3

