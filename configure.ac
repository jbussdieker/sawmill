AC_PREREQ([2.61])
AC_INIT(sawmill, 0.0.1)
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE
AC_CONFIG_FILES([Makefile])
AC_PROG_CC
AC_CHECK_LIB(pthread, pthread_create)

PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES(rabbitmq, [librabbitmq])

AC_OUTPUT
