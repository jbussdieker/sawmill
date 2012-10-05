# Sawmill

[![Build Status](https://secure.travis-ci.org/jbussdieker/sawmill.png)](http://travis-ci.org/jbussdieker/sawmill)

## Dependencies

Sawmill uses RabbitMQ-C hosted here: https://github.com/alanxz/rabbitmq-c

## Compiling

`````
./get_deps
autoreconf -i
./configure
make
`````

## Usage

Sawmill supports tailing files and stdin. The following two examples are equivalent.

`````
sawmill --host localhost --port 5673 /var/log/nginx/access.log /var/log/nginx/error.log
tail -f /var/log/nginx/access.log | sawmill --host localhost --port 5673 - /var/log/nginx/error.log
`````
