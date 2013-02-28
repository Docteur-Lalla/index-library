#!/bin/bash
# A script to compile index-library.

if [ $# = 0 ]
then
  echo "Compilation of index-add."
  g++ -std=c++0x -o bin/index-add src/index-core/* src/index-add/*

  echo "Compilation of index-search."
  g++ -std=c++0x -o bin/index-search src/index-core/* src/index-search/*

  echo "Compilation of index-remove."
  g++ -std=c++0x -o bin/index-remove src/index-core/* src/index-remove/*

  echo "Compilation of index-shell."
  g++ -std=c++0x -o bin/index-shell src/index-core/* src/index-shell/*

else
  echo "Compilation of ${1}."
  g++ -std=c++0x -o bin/$1 src/index-core/* src/$1/*
fi

