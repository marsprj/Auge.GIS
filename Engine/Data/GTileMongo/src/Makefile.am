AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libGTileMgo.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include \
		-I$(MONGO_C_HOME)/include/libbson-1.0 \
		-I$(MONGO_C_HOME)/include/libmongoc-1.0

libGTileMgo_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(MONGO_C_HOME)/lib

libGTileMgo_la_LIBADD=-lgcore -lmongoc-1.0 -lbson-1.0

libGTileMgo_la_SOURCES=\
		TileImpl.cpp \
		PGISTileStore.cpp \
		GoogleCRS84QuadTileStore.cpp \
		TileWorkspaceMongo.cpp \
		DataEngineTileMongo.cpp
