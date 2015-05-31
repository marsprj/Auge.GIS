AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libGTileFS.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libGTileFS_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src

libGTileFS_la_LIBADD=-lgcore

libGTileFS_la_SOURCES=\
		TileImpl.cpp \
		PGISTileStore.cpp \
		GoogleCRS84QuadTileStore.cpp \
		WorkspaceTFS.cpp \
		EnumTileStoreImpl.cpp \
		DataEngineImpl.cpp
