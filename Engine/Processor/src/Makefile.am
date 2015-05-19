AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgprocessor.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgprocessor_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src

libgprocessor_la_LIBADD=-lgcore

libgprocessor_la_SOURCES=\
		FileSytemTileStoreGeneratorImpl.cpp \
		ArchiveProcessorImpl.cpp \
		FeatureImportProcessorImpl.cpp \
		ProcessorFactoryImpl.cpp
