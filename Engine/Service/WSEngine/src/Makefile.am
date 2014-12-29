AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libWSEngine.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libWSEngine_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Service/WCore/src

libWSEngine_la_LIBADD=-lgcore

libWSEngine_la_SOURCES=\
		GetCapabilitiesRequest.cpp \
		GetCapabilitiesResponse.cpp \
		WSEngine.cpp

