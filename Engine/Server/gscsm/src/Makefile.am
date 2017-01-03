AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgscsm.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgscsm_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Server/gscore/src

libgscsm_la_LIBADD=-lgcore -lgscore

libgscsm_la_SOURCES=\
		CreateTaskHandler.cpp \
		CreateTaskRequest.cpp \
		CsTask.cpp \
		CsTaskManager.cpp \
		DescribeTaskHandler.cpp \
		DescribeTaskRequest.cpp \
		DescribeTaskResponse.cpp \
		EnumTask.cpp \
		JoinTaskHandler.cpp \
		JoinTaskRequest.cpp \
		WCSMapEngine.cpp
