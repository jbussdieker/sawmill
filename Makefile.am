bin_PROGRAMS = sawmill
sawmill_SOURCES = src/sawmill.c src/harvester.c src/amqp.c src/backoff.c
sawmill_LDADD = "-L./vendor/rabbitmq-c/librabbitmq/.libs -lrabbitmq"
AM_CPPFLAGS = "-Ivendor/rabbitmq-c/librabbitmq"

