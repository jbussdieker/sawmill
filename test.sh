#!/bin/bash
if [[ "x$1" == "xmon" ]]; then
    top -d 1 -p `ps -edf | grep sawmill | grep -v grep | awk '{print $2}'`
else
    ./build.sh && varnishncsa -a | ./bin/sawmill -d --exchange some_exchange --host localhost --port 56744 -
fi
