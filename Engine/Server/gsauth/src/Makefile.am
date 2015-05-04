AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgsauth.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgsauth_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Carto/src \
		-L$(top_srcdir)/Engine/Server/gscore/src

libgsauth_la_LIBADD=-lgcore -lgcarto -lgscore

libgsauth_la_SOURCES=\
		WAuthEngine.cpp

