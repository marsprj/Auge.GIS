AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libwservice.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libwservice_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Service/WCore/src

libwservice_la_LIBADD=-lgcore -lwcore

libwservice_la_SOURCES=\
		EnumServiceImpl.cpp \
		ServiceImpl.cpp  \
		ServiceManagerImpl.cpp \
		WebEngineManagerImpl.cpp
