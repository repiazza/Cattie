#!/usr/bin/bash

var=$(git describe --always --tags --dirty)

cmd="'/\\\$Id \\\$/c\ \* \\\$Id: "
cmd2=$var 
cmd3="\\\$' GXRF.h.teste"

echo $cmd$cmd2$cmd3

#sed $cmd$cmd2$cmd3

