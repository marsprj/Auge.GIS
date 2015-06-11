AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libGRasterDB.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include \
		-I/opt/gdal-1.11.12/include

libGRasterDB_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Raster/src \
		-L/opt/gdal-1.11.12/lib

libGRasterDB_la_LIBADD=-lgcore -lgraster

libGRasterDB_la_SOURCES=\
		RasterDatasetImpl.cpp \
		WorkspaceRaster.cpp \
		EnumDataSetImpl.cpp \
		DataEngineImpl.cpp
