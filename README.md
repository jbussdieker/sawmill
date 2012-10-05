# Sawmill

## Overview

[![Build Status](https://secure.travis-ci.org/jbussdieker/sawmill.png)](http://travis-ci.org/jbussdieker/sawmill)

Sawmill is a thin tool for tailing log files or STDIN and shipping messages off via AMQP for further processing.

## Install From Source

`````
./get_deps
autoreconf -i
./configure
make install
`````

## Build Package

`````
fpm -s dir -t deb -n sawmill -v 0.0.1 --prefix /usr/local --description "a log shipping tool" bin/sawmill
`````

## Usage

Sawmill supports tailing files and stdin. The following two examples are equivalent.

`````
sawmill --host localhost --port 5673 /var/log/nginx/access.log /var/log/nginx/error.log
tail -f /var/log/nginx/access.log | sawmill --host localhost --port 5673 - /var/log/nginx/error.log
`````
