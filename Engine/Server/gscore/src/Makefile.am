AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgscore.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgscore_la_LDFLAGS=-L$(top_srcdir)/Engine/Core/src

libgscore_la_LIBADD=-lgcore

libgscore_la_SOURCES=\
		WebContextImpl.cpp \
		WebEngineImpl.cpp \
		WebExceptionResponseImpl.cpp \
		WebSuccessResponseImpl.cpp \ 
		JobManagerImpl.cpp \
		JobImpl.cpp \
		WebWriterImpl.cpp
