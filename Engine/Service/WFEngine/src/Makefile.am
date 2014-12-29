AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libWFEngine.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libWFEngine_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Service/WCore/src

libWFEngine_la_LIBADD=-lgcore -lwcore

libWFEngine_la_SOURCES=\
		DescribeFeatureTypeRequest.cpp \
		GetCapabilitiesRequest.cpp \
		GetFeatureRequest.cpp \
		WFEngine.cpp \
		DescribeFeatureTypeResponse.cpp \
		GetCapabilitiesResponse.cpp \
		GetFeatureResponse.cpp

