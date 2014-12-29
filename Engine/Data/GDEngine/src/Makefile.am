AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgdengine.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgdengine_la_LDFLAGS=-L$(top_srcdir)/Engine/Core/src

libgdengine_la_LIBADD=-lgcore

libgdengine_la_SOURCES=\
		ConnectionManagerImpl.cpp \
		DataEngineManagerImpl.cpp
