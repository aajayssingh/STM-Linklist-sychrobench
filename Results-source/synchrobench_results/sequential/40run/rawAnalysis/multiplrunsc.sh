#!/bin/bash

x=1
while [ $x -le 160 ]
do
  ./sequential-linkedlist40
  sleep 1
  x=$(( $x + 1 ))
done
