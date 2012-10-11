#!/bin/bash
if [[ "x$1" == "xmon" ]]; then
    top -d 1 -p `ps -edf | grep sawmill | grep -v grep | awk '{print $2}'`
else
    ./build.sh && varnishncsa -a | ./bin/sawmill -f @type=varnishlog -f "@source=stdin://`hostname -f`/" -f @source_path=varnishncsa -f @source_host=`hostname -f` --exchange some_exchange --host localhost --port 5673 -
fi
