AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgtilemgo.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgtilemgo_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src

libgtilemgo_la_LIBADD=-lgcore

libgtilemgo_la_SOURCES=\
		PGISTileStore.cpp \
		GoogleCRS84QuadTileStore.cpp \
		TileWorkspaceMongo.cpp \
		DataEngineTileMongo.cpp
