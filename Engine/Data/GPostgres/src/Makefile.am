AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libGPostgres.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include \
		-I$(PGHOME)/include

libGPostgres_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Filter/src \
		-L$(PGHOME)/lib

libGPostgres_la_LIBADD=-lgcore -lgfilter -lpq

libGPostgres_la_SOURCES=\
		ConnectionPgs.cpp \
		DataEngineImpl.cpp \
		FeatureClassPgs.cpp \
		FeatureCursorPgs.cpp \
		FeatureInsertCommandPgs.cpp \
		FeaturePgs.cpp \
		FeatureObj.cpp \
		ResultSetPgs.cpp \
		SQLBuilder.cpp \
		WorkspacePgs.cpp
