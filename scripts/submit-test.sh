#!/bin/bash

#----------------------------------------------------------------------------#
# Submission script
# Output command lines without actually running matches
NEWPATH=$1
PREFIX=$2
MATCHCMD=$3

SCRIPT=$NEWPATH/$PREFIX.sh
echo "Test execution for: $NEWPATH/$PREFIX"
echo "$MATCHCMD"
