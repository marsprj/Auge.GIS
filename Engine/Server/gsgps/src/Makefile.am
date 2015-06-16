AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgsgps.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include \
		-L$(GEOS_HOME)/include

libgsgps_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Processor/src \
		-L$(top_srcdir)/Engine/Server/gscore/src \
		-L$(GEOS_HOME)/lib

libgsgps_la_LIBADD=-lgcore -lgscore -lgprocessor -lgeos_c

libgsgps_la_SOURCES=\
		threadpool.c \
		BuildPyramidHandler.cpp \
		BuildPyramidRequest.cpp \
		CapabilitiesHandler.cpp \
		CapabilitiesRequest.cpp \
		CapabilitiesResponse.cpp \
		CentroidHandler.cpp \
		CentroidRequest.cpp \
		CentroidResponse.cpp \
		BufferHandler.cpp \
		BufferRequest.cpp \
		BufferResponse.cpp \
		ThreadManager.cpp \
		TileUpdateHandler.cpp \
		TileUpdateRequest.cpp \
		FeatureImportRequest.cpp \
		FeatureImportHandler.cpp \
		GProcessEngine.cpp

