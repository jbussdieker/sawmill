bin_PROGRAMS = bin/sawmill
bin_sawmill_SOURCES = src/sawmill.c \
                      src/memory.c \
                      src/harvester.c \
                      src/emitter.c \
                      src/amqp.c \
                      src/backoff.c
bin_sawmill_LDADD = $(rabbitmq_LIBS)
AM_CPPFLAGS = $(rabbitmq_CFLAGS)

TESTS = test/memory_leak.test

clean-local:
	rm *.deb

# Debian packaging
deb:$(bin_PROGRAMS)
	fpm -s dir -t deb -n sawmill -v $(VERSION) --prefix $(prefix) --description "a log shipping tool" $(bin_PROGRAMS)
