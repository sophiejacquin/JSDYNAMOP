#!/bin/bash
PCROSS=$3
PMUT=$5
MUT1=$7
PHYB=$9
PT=${11}
TM=${13}
POP=${15}
PROFHYB=${17}
THYB=${19}

CMD="./OneMachineEA2" 

$CMD --file=$1 --pCross=$PCROSS --pMut=$PMUT --mut1Rate=$MUT1 --pHyb=$PHYB --pTiming=$PT --tMut=$TM --popSize=$POP --profHyb=$PROFHYB --tailleHyb=$THYB -w1 --temps=400 -G1000000000
