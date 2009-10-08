#!/bin/bash

#----------------------------------------------------------------------------#
# Run multiple versions of RLGO using various settings, 
# then benchmark in an Elo tournament and finally plot results
if [ $# -lt 7 ]
then
    echo "Usage:"
    echo "experiment.sh train-player test-player path setting values title games submit [options]"
    echo ""
    echo "train-player:   Tag used to identify training player in the getprogram.sh script"
    echo "test-player:    Tag used to identify testing player in the getprogram.sh script"
    echo "path:     Path where match data should be saved"
    echo "setting:  Setting to vary in player"
    echo "values:   List of values to try for setting, e.g. \"0.1 0.2 0.3\""
    echo "title:    Title of the plot"
    echo "games:    Number of games to run"
    echo "options:  Any set of options supported by RLGO, passed to both train and test player"
    exit 1
fi

SCRIPTDIR=`dirname $0`
TRAINPLAYER=$1
TESTPLAYER=$2
PATHSTEM=$3
SETTING=$4
VALUES=$5
TITLE=$6
GAMES=$7
shift; shift; shift; shift; shift; shift; shift
OPTIONS=$@

$SCRIPTDIR/multi-run.sh $TRAINPLAYER $PATHSTEM $SETTING "$VALUES" $GAMES seqplay.sh $OPTIONS
$SCRIPTDIR/generate-players.sh $TESTPLAYER $PATHSTEM $SETTING "$VALUES"
$SCRIPTDIR/tournament.sh $PATHSTEM/short-names.txt $PATHSTEM/program-names.txt $PATHSTEM 9 0 2 100000 1
$SCRIPTDIR/analyze-tournament.sh $PATHSTEM/short-names.txt $PATHSTEM
$SCRIPTDIR/plot-tournament.sh $PATHSTEM $SETTING "$VALUES" "$TITLE"
$SCRIPTDIR/plot-tournament2.sh $PATHSTEM $SETTING "$VALUES" "$TITLE"
