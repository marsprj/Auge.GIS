AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgshape.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgshape_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Filter/src

libgshape_la_LIBADD=-lgcore -lgfilter

libgshape_la_SOURCES=\
		DataEngineImpl.cpp \
		dbfopen.cpp \
		FeatureClassShp.cpp \
		FeatureCursorShp.cpp \
		FeatureShp.cpp \
		shpopen.cpp \
		ShpUtil.cpp \
		WorkspaceShp.cpp
