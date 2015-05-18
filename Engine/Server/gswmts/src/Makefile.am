AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgswmts.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgswmts_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Carto/src \
		-L$(top_srcdir)/Engine/Server/gscore/src

libgswmts_la_LIBADD=-lgcore -lgcarto -lgscore

libgswmts_la_SOURCES=\
		CapabilitiesHandler.cpp \
		CapabilitiesRequest.cpp \
		CapabilitiesResponse.cpp \
		GetTileHandler.cpp \
		GetTileRequest.cpp \
		GetTileResponse.cpp \
		WTileEngine.cpp \
		WTileRequest.cpp


