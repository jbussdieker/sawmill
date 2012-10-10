AC_PREREQ([2.61])
AC_INIT(sawmill, 0.1.6)
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE
AC_CONFIG_FILES([Makefile])
AC_PROG_CC
AC_CHECK_LIB(pthread, pthread_create)
AC_ARG_WITH([rabbitmq-include-path],
    [AS_HELP_STRING([--with-rabbitmq-include-path],
        [location of the rabbitmq-c headers, defaults to vendor/rabbitmq-c])],
    [rabbitmq_CFLAGS="-$withval"],
    [rabbitmq_CFLAGS="-Ivendor/rabbitmq-c/librabbitmq"])
AC_SUBST([rabbitmq_CFLAGS])
AC_ARG_WITH([rabbitmq-lib-path],
    [AS_HELP_STRING([--with-rabbitmq-lib-path],
        [location of the rabbitmq-c libraries, defaults to vendor/rabbitmq-c])],
    [rabbitmq_LIBS="-L$withval -lrabbitmq"],
    [rabbitmq_LIBS='-L./vendor/rabbitmq-c/librabbitmq/.libs -lrabbitmq'])
AC_SUBST([rabbitmq_LIBS])

AC_OUTPUT
