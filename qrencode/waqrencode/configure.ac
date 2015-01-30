#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([waqrencode], [0.1], [keisuke.miyako@4d.com])
AM_INIT_AUTOMAKE()
AC_CONFIG_SRCDIR([waqrencode.cc])
# AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

AC_CHECK_LIB([png], [png_set_option])

AC_CHECK_LIB([qrencode], [QRcode_free])

AC_CHECK_LIB([z], [zlibVersion])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h limits.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset strcasecmp])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
