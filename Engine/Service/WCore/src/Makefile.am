AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libwcore.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libwcore_la_LDFLAGS=-L$(top_srcdir)/Engine/Core/src

libwcore_la_LIBADD=-lgcore

libwcore_la_SOURCES=\
		WebContextImpl.cpp \
		WebEngineImpl.cpp \
		WebExceptionResponseImpl.cpp \
		WebWriterImpl.cpp
