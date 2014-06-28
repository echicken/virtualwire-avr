#!/bin/bash

for i in build/*_test
do
  echo -n "${i#build/} "
  if $i > test.log
  then
    echo -e "\033[0;37m[\033[1;32mPASS\033[0;37m]\033[0m"
  else
    echo -e "\033[0;37m[\033[0;31mFAIL\033[0;37m]\033[0m"
    cat test.log
  fi
done
