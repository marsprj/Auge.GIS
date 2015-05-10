AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libGRasterDB.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libGRasterDB_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Data/GRaster\src

libGRasterDB_la_LIBADD=-lgcore -lgdal -lgraster

libGRasterDB_la_SOURCES=\
		RasterBandImpl.cpp \
		WorkspaceRaster.cpp \
		DataEngineImpl.cpp
