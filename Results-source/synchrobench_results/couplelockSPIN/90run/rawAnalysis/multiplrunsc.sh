#!/bin/bash

x=1
while [ $x -le 160 ]
do
  ./SPIN-hoh-list
  sleep 5
  x=$(( $x + 1 ))
done
