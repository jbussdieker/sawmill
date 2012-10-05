[ -d vendor/rabbitmq-c ] || ./get_deps
[ -f Makefile.in ] || autoreconf -i
[ -f Makefile ] || ./configure
make
