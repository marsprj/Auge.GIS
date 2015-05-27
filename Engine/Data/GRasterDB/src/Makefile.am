AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgrasterdb.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include \
		-I/opt/gdal-1.11.12/include

libgrasterdb_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Data/GRaster/src \
		-L/opt/gdal-1.11.12/lib

libgrasterdb_la_LIBADD=-lgcore -lgdal -lgraster

libgrasterdb_la_SOURCES=\
		RasterDatasetImpl.cpp \
		WorkspaceRaster.cpp \
		DataEngineImpl.cpp
