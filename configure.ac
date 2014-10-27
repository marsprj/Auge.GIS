#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Auge], [1.00], [marsprj@gmail.com])
AC_CONFIG_SRCDIR([Brush/manager/test.cpp])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(Auge,1.0.0)
AC_PROG_LIBTOOL

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lpg':
AC_CHECK_LIB([pg], [main])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.

AC_CONFIG_FILES([Brush/Makefile
                 Brush/src/Makefile
                 Makefile])
AC_OUTPUT
