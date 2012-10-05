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

You can also specify custom fields to add:

`````
sawmill --field @type=nginxlog --field @source=appserver01 --host localhost --port 5673 /var/log/nginx/access.log
`````

which would produce messages like this:

`````
{"@fields":{"message":"1.2.3.4 - - [12/Sept/1983:00:29:26 -0700] \"GET /awesome HTTP/0.1\" 301 173 \"-\" \"browser/0.3\""},"@type":"nginxlog","@source":"appserver01"}
`````
