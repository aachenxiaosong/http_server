#!/bin/bash

export LD_LIBRARY_PATH=./lib/libevent:./lib/mqtt_embed:./lib/mqtt
./http_server
