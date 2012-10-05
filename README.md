# Sawmill

## Overview

[![Build Status](https://secure.travis-ci.org/jbussdieker/sawmill.png)](http://travis-ci.org/jbussdieker/sawmill)

Sawmill is a thin tool for tailing log files or STDIN and shipping messages off via AMQP for further processing.

## Install From Source

`````
./build.sh
make install
`````

## Build Package

`````
./build.sh
make deb
`````

## Usage

Sawmill supports tailing files and stdin. The following two examples are equivalent.

`````
sawmill --host localhost --port 5673 /var/log/nginx/access.log /var/log/nginx/error.log
tail -f /var/log/nginx/access.log | sawmill --host localhost --port 5673 - /var/log/nginx/error.log
`````
