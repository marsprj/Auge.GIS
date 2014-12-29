AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgpostgres.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include \
		-I$(PG_HOME)/include

libgpostgres_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Filter/src \
		-L$(PG_HOME)/lib

libgpostgres_la_LIBADD=-lgcore -lgfilter -lpq

libgpostgres_la_SOURCES=\
		ConnectionPgs.cpp \
		DataEngineImpl.cpp \
		FeatureClassPgs.cpp \
		FeatureCursorPgs.cpp \
		FeatureInsertCommandPgs.cpp \
		FeaturePgs.cpp \
		Makefile.am
		ResultSetPgs.cpp \
		SQLBuilder.cpp \
		WorkspacePgs.cpp
