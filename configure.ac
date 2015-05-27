#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

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
# FIXME: Replace `main' with a function in `-lgsims':
AC_CHECK_LIB([gsims], [main])
# FIXME: Replace `main' with a function in `-llog4cplus':
AC_CHECK_LIB([log4cplus], [main])
# FIXME: Replace `main' with a function in `-lpq':
AC_CHECK_LIB([pq], [main])
# FIXME: Replace `main' with a function in `-lrudecgi':
AC_CHECK_LIB([rudecgi], [main])
# FIXME: Replace `main' with a function in `-luuid':
AC_CHECK_LIB([uuid], [main])
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
		 Engine/Data/GRaster/Makefile
                 Engine/Data/GRaster/src/Makefile
		 Engine/Data/GRasterFS/Makefile
                 Engine/Data/GRasterFS/src/Makefile
		 Engine/Data/GRasterDB/Makefile
                 Engine/Data/GRasterDB/src/Makefile
                 Engine/Data/GTileFS/Makefile
                 Engine/Data/GTileFS/src/Makefile
                 Engine/Data/GTileMongo/Makefile
                 Engine/Data/GTileMongo/src/Makefile
                 Engine/Data/Makefile
                 Engine/Processor/Makefile
                 Engine/Processor/src/Makefile
                 Engine/Filter/Makefile
                 Engine/Auth/src/Makefile
		 Engine/Auth/Makefile
                 Engine/Filter/src/Makefile
                 Engine/Makefile
                 Engine/Server/Makefile
                 Engine/Server/gscore/Makefile
                 Engine/Server/gscore/src/Makefile
                 Engine/Server/gsims/Makefile
                 Engine/Server/gsims/src/Makefile
                 Engine/Server/gsvrs/Makefile
                 Engine/Server/gsvrs/src/Makefile
                 Engine/Server/gswfs/Makefile
                 Engine/Server/gswfs/src/Makefile
                 Engine/Server/gswms/Makefile
                 Engine/Server/gswms/src/Makefile
                 Engine/Server/gswmts/Makefile
                 Engine/Server/gswmts/src/Makefile
                 Engine/Server/gsdbs/Makefile
                 Engine/Server/gsdbs/src/Makefile
                 Engine/Server/gsgps/Makefile
                 Engine/Server/gsgps/src/Makefile
		 Engine/Server/gsauth/Makefile
                 Engine/Server/gsauth/src/Makefile
                 Engine/Server/ims/Makefile
                 Engine/Server/mgr/Makefile
                 Engine/Server/testcgi/Makefile
                 Makefile
                 include/Makefile])
AC_OUTPUT
