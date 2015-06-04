AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libGRasterFS.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include \
		-I/opt/gdal-1.11.12/include

libGRasterFS_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L/opt/gdal-1.11.12/lib

libGRasterFS_la_LIBADD=-lgcore

libGRasterFS_la_SOURCES=\
		RasterDatasetImpl.cpp \
		WorkspaceRaster.cpp \
		DataEngineImpl.cpp
