#!/bin/bash

x=1
while [ $x -le 160 ]
do
  ./MUTEX-hoh-list
  sleep 1
  x=$(( $x + 1 ))
done
