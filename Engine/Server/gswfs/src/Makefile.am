AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgswfs.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgswfs_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Service/WCore/src

libgswfs_la_LIBADD=-lgcore -lwcore

libgswfs_la_SOURCES=\
		CapabilitiesHandler.cpp \
		CapabilitiesRequest.cpp \
		CapabilitiesResponse.cpp \
		DescribeFeatureTypeHandler.cpp \
		DescribeFeatureTypeRequest.cpp \
		DescribeFeatureTypeResponse.cpp \
		GetFeatureHandler.cpp \
		GetFeatureRequest.cpp \
		GetFeatureResponse.cpp \
		WFeatureEngine.cpp

