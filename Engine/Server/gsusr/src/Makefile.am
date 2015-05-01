AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgsusr.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgsusr_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Carto/src \
		-L$(top_srcdir)/Engine/Server/gscore/src

libgsusr_la_LIBADD=-lgcore -lgcarto -lgscore

libgsusr_la_SOURCES=\
		WUserEngine.cpp

