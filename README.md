# Sawmill

## Overview

[![Build Status](https://secure.travis-ci.org/jbussdieker/sawmill.png)](http://travis-ci.org/jbussdieker/sawmill)

Sawmill is a thin tool for tailing log files and/or stdin and shipping messages off via AMQP for Logstash (http://logstash.net/) processing.

## Installing

### From Source

`````
./build.sh
sudo make install
`````

### From Package

`````
./build.sh
make deb
sudo dpkg -i sawmill*.deb
`````

## Usage

### Logstash

The following logstash config is useful for debugging.

`````
input {
  amqp {
    name => "some_queue"
    host => "localhost"
    port => 5673
    user => "guest"
    password => "guest"
    exchange => "some_exchange"
    type => "all"
  }
}
output {
  stdout {
    debug => true
  }
}
`````

### Recipes

Sawmill supports tailing files and stdin (identified by a single - ). The following two examples are equivalent.

`````
sawmill -e some_exchange -host localhost -port 5673 /var/log/nginx/access.log /var/log/nginx/error.log
tail -f /var/log/nginx/access.log | sawmill -host localhost -port 5673 - /var/log/nginx/error.log
`````

You can also specify custom fields to add:

`````
sawmill -e some_exchange -f @type=nginxlog -f @source=appserver01 -host localhost -port 5673 /var/log/nginx/access.log
`````

which would produce messages like this:

`````
{"@fields":{"message":"[some line from the log]"},"@type":"nginxlog","@source":"appserver01"}
`````
