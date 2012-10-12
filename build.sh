[ -f vendor/rabbitmq-c/librabbitmq/amqp.h ] || ./get_deps
[ -f Makefile.in ] || autoreconf -i
[ -f Makefile ] || ./configure
make
