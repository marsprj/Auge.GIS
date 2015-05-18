AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgtilemgo.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include \
		-I$(MONGO_C_HOME)/include/libbson-1.0 \
		-I$(MONGO_C_HOME)/include/libmongoc-1.0

libgtilemgo_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src

libgtilemgo_la_LIBADD=-lgcore -lmongoc-1.0 -lbson-1.0

libgtilemgo_la_SOURCES=\
		PGISTileStore.cpp \
		GoogleCRS84QuadTileStore.cpp \
		WorkspaceTileMongo.cpp \
		DataEngineTileMongo.cpp
