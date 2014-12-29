AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libWMEngine.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libWMEngine_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Carto/src \
		-L$(top_srcdir)/Engine/Service/WCore/src

libWMEngine_la_LIBADD=-lgcore -lgcarto -lwcore

libWMEngine_la_SOURCES=\
		CreateMapRequest.cpp \
		CreateMapResponse.cpp \
		GetCapabilitiesRequest.cpp \
		GetCapabilitiesResponse.cpp \
		GetMapRequest.cpp \
		GetMapResponse.cpp \
		WMEngine.cpp
