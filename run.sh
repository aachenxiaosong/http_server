#!/bin/bash

export LD_LIBRARY_PATH=./lib/libevent:./lib/poco:./lib/boost:./lib/mqtt:./lib/zmq
./uni_gateway
