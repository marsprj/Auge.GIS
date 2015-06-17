AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgprocessor.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include \
		-I$(ARCHIVE_HOME)/include

libgprocessor_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(ARCHIVE_HOME)/lib

libgprocessor_la_LIBADD=-lgcore -larchive

libgprocessor_la_SOURCES=\
		BufferProcessorImpl.cpp \
		FileSytemTileStoreGeneratorImpl.cpp \
		MongoTileStoreGenerator.cpp \
		ArchiveProcessorImpl.cpp \
		FeatureImportProcessorImpl.cpp \
		ProcessorFactoryImpl.cpp \
		CsvImportProcessorImpl.cpp
