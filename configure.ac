AC_INIT([liboindirect],[0.1],[avati@gluster.com])

AM_INIT_AUTOMAKE

AC_CONFIG_FILES([Makefile])

AC_CANONICAL_HOST

AC_PROG_CC

AC_CHECK_TOOL([LD],[ld])

AC_CHECK_LIB([pthread], [pthread_mutex_init], ,AC_MSG_ERROR([Posix threads library is required]))

AC_OUTPUT
