AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgraster.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include \
		-I/opt/gdal-1.11.12/include

libgraster_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Filter/src \
		-L/opt/gdal-1.11.12/lib

libgraster_la_LIBADD=-lgcore -lgdal

libgraster_la_SOURCES=\
		RasterImpl.cpp \
		RasterBandImpl.cpp \
		RasterIOImpl.cpp
