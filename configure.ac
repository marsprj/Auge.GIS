#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Auge.GIS], [0.1.0], [admin@mapcloud.org])
AC_CONFIG_SRCDIR([include/wkb.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE
AC_PROG_LIBTOOL


# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lxml2':
AC_CHECK_LIB([xml2], [main])

# Checks for header files.
AC_CHECK_HEADERS([limits.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([floor getcwd memset mkdir strchr strdup strrchr strstr])

AC_CONFIG_FILES([Engine/Core/Makefile
                 Engine/Core/src/Makefile
                 Engine/Makefile
 		 Engine/Filter/Makefile
                 Engine/Filter/src/Makefile
		 Engine/Data/Makefile
		 Engine/Data/GDEngine/Makefile
                 Engine/Data/GDEngine/src/Makefile
		 Engine/Data/GPostgres/Makefile
                 Engine/Data/GPostgres/src/Makefile
                 Engine/Data/GShape/Makefile
                 Engine/Data/GShape/src/Makefile
                 Makefile
                 include/Makefile])
AC_OUTPUT
