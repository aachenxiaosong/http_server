#!/bin/bash

export LD_LIBRARY_PATH=./lib/libevent:./lib/poco:./lib/mqtt
./http_server
