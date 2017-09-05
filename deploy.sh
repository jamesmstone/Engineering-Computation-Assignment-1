#!/bin/sh
set -e

server(){
  ssh stone1@dimefox.eng.unimelb.edu.au. "$@"
}

scp ./assmt1.c stone1@dimefox.eng.unimelb.edu.au:
server submit comp20005 a1 assmt1.c
sleep 60
server verify comp20005 a1

