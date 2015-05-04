AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgauth.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgauth_la_LDFLAGS=-L$(top_srcdir)/Engine/Core/src

libgauth_la_LIBADD=-lgcore

libgauth_la_SOURCES=\
		RoleImpl.cpp \
		UserImpl.cpp \
		EnumRoleImpl.cpp \
		EnumUserImpl.cpp \
		UserManagerImpl.cpp
