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
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lGPostgres':
AC_CHECK_LIB([GPostgres], [main])
# FIXME: Replace `main' with a function in `-lGShape':
AC_CHECK_LIB([GShape], [main])
# FIXME: Replace `main' with a function in `-lcairo':
AC_CHECK_LIB([cairo], [main])
# FIXME: Replace `main' with a function in `-lcppunit':
AC_CHECK_LIB([cppunit], [main])
# FIXME: Replace `main' with a function in `-lfcgi':
AC_CHECK_LIB([fcgi], [main])
# FIXME: Replace `main' with a function in `-lgcarto':
AC_CHECK_LIB([gcarto], [main])
# FIXME: Replace `main' with a function in `-lgcore':
AC_CHECK_LIB([gcore], [main])
# FIXME: Replace `main' with a function in `-lgdengine':
AC_CHECK_LIB([gdengine], [main])
# FIXME: Replace `main' with a function in `-lgfilter':
AC_CHECK_LIB([gfilter], [main])
# FIXME: Replace `main' with a function in `-llog4cplus':
AC_CHECK_LIB([log4cplus], [main])
# FIXME: Replace `main' with a function in `-lpq':
AC_CHECK_LIB([pq], [main])
# FIXME: Replace `main' with a function in `-lrudecgi':
AC_CHECK_LIB([rudecgi], [main])
# FIXME: Replace `main' with a function in `-lwcore':
AC_CHECK_LIB([wcore], [main])
# FIXME: Replace `main' with a function in `-lwservice':
AC_CHECK_LIB([wservice], [main])
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

AC_CONFIG_FILES([Dist/32_x86_win_vc10/Makefile
                 Dist/32_x86_win_vc10/binD/Makefile
                 Dist/32_x86_win_vc10/binD/conf/Makefile
                 Dist/Makefile
                 Engine/Carto/Makefile
                 Engine/Carto/src/Makefile
                 Engine/Carto/test/Makefile
                 Engine/Core/Makefile
                 Engine/Core/src/Makefile
                 Engine/Data/GDEngine/Makefile
                 Engine/Data/GDEngine/src/Makefile
                 Engine/Data/GPostgres/Makefile
                 Engine/Data/GPostgres/src/Makefile
                 Engine/Data/GPostgres/test/Makefile
                 Engine/Data/GShape/Makefile
                 Engine/Data/GShape/src/Makefile
                 Engine/Data/GShape/test/Makefile
                 Engine/Data/Makefile
                 Engine/Filter/Makefile
                 Engine/Filter/src/Makefile
                 Engine/Makefile
                 Engine/Server/Makefile
                 Engine/Server/gswfs/Makefile
                 Engine/Server/gswfs/src/Makefile
                 Engine/Server/gswms/Makefile
                 Engine/Server/gswms/src/Makefile
                 Engine/Server/ims/Makefile
                 Engine/Service/Makefile
                 Engine/Service/WCore/Makefile
                 Engine/Service/WCore/src/Makefile
                 Engine/Service/WService/Makefile
                 Engine/Service/WService/src/Makefile
                 Engine/Service/ims/Makefile
                 Makefile
                 include/Makefile])
AC_OUTPUT
