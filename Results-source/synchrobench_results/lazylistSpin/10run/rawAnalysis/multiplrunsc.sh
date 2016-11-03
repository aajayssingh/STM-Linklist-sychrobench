#!/bin/bash

x=1
while [ $x -le 160 ]
do
  ./SPIN-lazy-list
  sleep 3
  x=$(( $x + 1 ))
done
