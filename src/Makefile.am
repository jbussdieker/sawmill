bin_PROGRAMS = sawmill
sawmill_SOURCES = sawmill.c \
		harvester.c \
		amqp_emitter.c \
		memory.c \
		emitter.c \
		backoff.c
sawmill_LDADD = $(rabbitmq_LIBS)
AM_CPPFLAGS = $(rabbitmq_CFLAGS)
