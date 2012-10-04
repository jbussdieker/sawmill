bin_PROGRAMS = sawmill
sawmill_SOURCES = src/sawmill.c src/harvester.c src/amqp.c src/backoff.c
sawmill_LDADD = $(rabbitmq_LIBS)
AM_CPPFLAGS = $(rabbitmq_CFLAGS)

