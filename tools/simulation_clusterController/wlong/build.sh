#!/bin/bash

rm -rf uni_ele_wlong_test
g++ -o uni_ele_wlong_test uni_ele_wlong_test.cpp -I ../../../lib/libevent/inc -L../../../lib/libevent -levent-2.2  -lpthread
