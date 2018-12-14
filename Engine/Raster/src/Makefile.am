AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgraster.la
AM_CXXFLAGS=-D_LINUX  -std=c++11

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include \
		-I$(GDAL_HOME)/include

libgraster_la_LDFLAGS=-L$(GDAL_HOME)/lib

libgraster_la_LIBADD=-lgdal

libgraster_la_SOURCES=\
		RasterImpl.cpp \
		RasterBandImpl.cpp \
		RasterIOImpl.cpp \
		RasterFactoryImpl.cpp
