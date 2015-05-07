AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgraster.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgraster_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Filter/src

libgraster_la_LIBADD=-lgcore -lgdal

libgraster_la_SOURCES=\
		RasterImpl.cpp \
		RasterBandImpl.cpp \
		WorkspaceRaster.cpp \
		DataEngineImpl.cpp
