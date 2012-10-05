bin_PROGRAMS = bin/sawmill
bin_sawmill_SOURCES = src/sawmill.c src/harvester.c src/amqp.c src/backoff.c
bin_sawmill_LDADD = $(rabbitmq_LIBS)
AM_CPPFLAGS = $(rabbitmq_CFLAGS)
deb:
	fpm -s dir -t deb -n sawmill -v $(VERSION) --prefix $(prefix) --description "a log shipping tool" $(bin_PROGRAMS)
