bin_PROGRAMS = sawmill
sawmill_SOURCES = sawmill.c harvester.c emitter.c amqp.c backoff.c
sawmill_LDADD = $(rabbitmq_LIBS)
AM_CPPFLAGS = $(rabbitmq_CFLAGS)
