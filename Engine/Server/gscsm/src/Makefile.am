AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgscsm.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgscsm_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Carto/src \
		-L$(top_srcdir)/Engine/Server/gscore/src

libgscsm_la_LIBADD=-lgcore -lgcarto -lgscore

libgscsm_la_SOURCES=\
		CapabilitiesHandler.cpp \
		CapabilitiesRequest.cpp \
		CapabilitiesResponse.cpp \
		GetFeatureInfoHandler.cpp \
		GetFeatureInfoRequest.cpp \
		GetFeatureInfoResponse.cpp \
		GetMapHandler.cpp \
		GetMapRequest.cpp \
		GetMapResponse.cpp \
		WMapEngine.cpp \
		SetStyleHandler.cpp \
		WMapRequest.cpp \
		SetStyleRequest.cpp

