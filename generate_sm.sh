#!/bin/bash
cd "$(dirname "$0")" # change to current script directory

SRC=lib/fa/src

reset

# run user code generation script.
$SRC/fa_sm.csx #--isolated-load-context
# can also run c# script with this command instead: dotnet script $SRC/Tutorial1Sm.csx

rm $SRC/fa_sm_gen.cpp
mv $SRC/fa_sm_gen.c $SRC/fa_sm_gen.cpp
