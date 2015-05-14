AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libGProcessor.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libGProcessor_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src

libGProcessor_la_LIBADD=-lgcore

libGProcessor_la_SOURCES=\
		FileSytemTileStoreGeneratorImpl.cpp \
		ProcessorFactoryImpl.cpp
