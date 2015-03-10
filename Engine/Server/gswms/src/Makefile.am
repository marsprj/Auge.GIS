AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgswms.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgswms_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Carto/src \
		-L$(top_srcdir)/Engine/Service/WCore/src

libgswms_la_LIBADD=-lgcore -lgcarto -lwcore

libgswms_la_SOURCES=\
		CapabilitiesHandler.cpp \
		CapabilitiesRequest.cpp \
		CapabilitiesResponse.cpp \
		GetFeatureInfoHandler.cpp \
		GetFeatureInfoRequest.cpp \
		GetFeatureInfoResponse.cpp \
		GetMapHandler.cpp \
		GetMapRequest.cpp \
		GetMapResponse.cpp \
		WMapEngine.cpp

