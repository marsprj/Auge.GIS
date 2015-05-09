AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libGRaster.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libGRaster_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Filter/src

libGRaster_la_LIBADD=-lgcore -lgdal

libGRaster_la_SOURCES=\
		RasterImpl.cpp \
		RasterBandImpl.cpp \
		WorkspaceRaster.cpp \
		DataEngineImpl.cpp
