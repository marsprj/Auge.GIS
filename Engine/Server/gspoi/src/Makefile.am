AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgspoi.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgspoi_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Server/gscore/src \
		-L$(top_srcdir)/Engine/Server/gsvrs/src

libgspoi_la_LIBADD=-lgcore -lgscore -lgsvrs

libgspoi_la_SOURCES=\		    
		    WPoiEngine.cpp

