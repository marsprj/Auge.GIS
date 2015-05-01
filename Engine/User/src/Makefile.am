AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libguser.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libguser_la_LDFLAGS=-L$(top_srcdir)/Engine/Core/src

libguser_la_LIBADD=-lgcore

libguser_la_SOURCES=\
		RoleImpl.cpp \
		UserImpl.cpp \
		EnumRoleImpl.cpp \
		EnumUserImpl.cpp \
		UserManagerImpl.cpp
