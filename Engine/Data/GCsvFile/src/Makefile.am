AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libGCsvFile.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libGCsvFile_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src

libGCsvFile_la_LIBADD=-lgcore

libGCsvFile_la_SOURCES=\
		DataEngineImpl.cpp \
		CursorImpl.cpp \
		DataSetCvs.cpp \
		RowImpl.cpp \
		WorkspaceCsv.cpp
