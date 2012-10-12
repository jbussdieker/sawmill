#!/bin/sh
[ -f vendor/rabbitmq-c/librabbitmq/.libs/librabbitmq.a ] || ./get_deps
[ -f Makefile.in ] || autoreconf -i
[ -f Makefile ] || ./configure
make
