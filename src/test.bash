#!/bin/bash
j=2
while [ $j -le 300 ]; do 
i=1
s=81
while [ $i -le 50 ]; do 
var=testE${i}_${j}
  ./OneMachineEA --file="/home/jacquin/Bureau/OneMachine/bky20/bky20_${j}.txt" -C0.9326 --pMut=0.8726 --mut1Rate=0.499 --pHyb=0.0498 --pTiming=0.7239 --tMut=16 --popSize=127 --profHyb=2762 --tailleHyb=8 -G10000000000 --temps=4 -w1 --seed=$s >>Test20_4/$var
STDOUT="Test20_4/$var"
COST=$(tail -1 ${STDOUT} | cut -d ':' -f 2)
echo "$COST" >>Test20_4/result_${j}
echo "$COST" $var fait

let $[ s+=100 ]
let $[ i+=1 ] # incremente i de 1 a chaque boucle 
done
let $[ j+=1 ]
done
j=10
while [ $j -le 300 ]; do 
i=1
s=81
while [ $i -le 50 ]; do 
var=testE${i}_${j}
  ./OneMachineEA --file="/home/jacquin/Bureau/OneMachine/bky200/pourTests/bky200_${j}.txt" --pCross=0.2718 --pMut=0.6769 --mut1Rate=0.785 --pHyb=0.0209 --pTiming=0.2451 --tMut=138 --popSize=147 --profHyb=52 --tailleHyb=7 -G10000000000 --temps=400 -w1 --seed=$s >>Test200/$var
STDOUT="Test200/$var"
COST=$(tail -1 ${STDOUT} | cut -d ':' -f 2)
echo "$COST" >>Test200/result_${j}
echo "$COST" $var fait

let $[ s+=100 ]
let $[ i+=1 ] # incremente i de 1 a chaque boucle 
done
let $[ j+=10 ]
done


