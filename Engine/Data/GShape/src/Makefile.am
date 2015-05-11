AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libGShape.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libGShape_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Filter/src

libGShape_la_LIBADD=-lgcore -lgfilter

libGShape_la_SOURCES=\
		DataEngineImpl.cpp \
		dbfopen.cpp \
		FeatureClassShp.cpp \
		FeatureCursorShp.cpp \
		FeatureShp.cpp \
		FeatureObj.cpp \
		shpopen.cpp \
		ShpUtil.cpp \
		FeatureInsertCommandShp.cpp \
		WorkspaceShp.cpp
