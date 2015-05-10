AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libGRasterFS.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libGRasterFS_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Data/GRaster\src

libGRasterFS_la_LIBADD=-lgcore -lgdal -lgraster

libGRasterFS_la_SOURCES=\
		RasterBandImpl.cpp \
		WorkspaceRaster.cpp \
		DataEngineImpl.cpp
