AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgsvrs.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgsvrs_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Carto/src \
		-L$(top_srcdir)/Engine/Server/gscore/src

libgsvrs_la_LIBADD=-lgcore -lgscore -lgcarto

libgsvrs_la_SOURCES=\
		EnumServiceImpl.cpp \
		ServiceImpl.cpp  \
		ServiceManagerImpl.cpp \
		WebEngineManagerImpl.cpp \
		EnumWebEngineImpl.cpp
