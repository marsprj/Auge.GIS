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
		DescribeFeatureTypeHandler.cpp \
		DescribeFeatureTypeRequest.cpp \
		DescribeFeatureTypeResponse.cpp \
		GetFeatureHandler.cpp \
		GetFeatureRequest.cpp \
		GetFeatureResponse.cpp \
		GetGmlObjectHandler.cpp \
		GetGmlObjectRequest.cpp \
		GetGmlObjectResponse.cpp \
		TransactionHandler.cpp \
		TransactionRequest.cpp \
		TransactionResponse.cpp \
		WFeatureEngine.cpp
