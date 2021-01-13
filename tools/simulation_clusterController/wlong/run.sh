#!/bin/bash

export LD_LIBRARY_PATH=../../../lib/libevent
chmod 777 uni_ele_wlong_test

if [ $# -lt 1 ]; then
  echo "usage: ./uni_ele_wlong_test port"
fi
./uni_ele_wlong_test $1
