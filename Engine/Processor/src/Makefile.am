AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgprocessor.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include \
		-I$(ARCHIVE_HOME)/include \
		-I$(PROJ4_HOME)/include

libgprocessor_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Raster/src \
		-L$(ARCHIVE_HOME)/lib \
		-L$(PROJ4_HOME)/lib

libgprocessor_la_LIBADD=-lgcore -larchive -lgraster -lproj

libgprocessor_la_SOURCES=\
		BufferProcessorImpl.cpp \
		FileSytemTileStoreGeneratorImpl.cpp \
		MongoTileStoreGenerator.cpp \
		ArchiveProcessorImpl.cpp \
		FeatureImportProcessorImpl.cpp \
		FeatureExportProcessorImpl.cpp \
		ProcessorFactoryImpl.cpp \
		CsvImportProcessorImpl.cpp \
		XYImportProcessorImpl.cpp \
		FeatureProjectProcessorImpl.cpp \
		MultiPointToPointsProcessorImpl.cpp \
		LineToPointsProcessorImpl.cpp \
		PolygonToPointsProcessorImpl.cpp \
		PolygonToLinieProcessorImpl.cpp \
		RandomPointsGeneratorImpl.cpp \
		RandomPointsInPolygonGeneratorImpl.cpp \
		KmeanProcessorImpl.cpp \
		Kmean.cpp \
		DBScanProcessorImpl.cpp \
		DBScan.cpp \
		SrsBase.cpp \
		sqlite3.c \
		RasterExtractByRectangleProcessorImpl.cpp \
		RasterEdgeDetectProcessorImpl.cpp \
		RasterStretchProcessorImpl.cpp \
		RasterHistogramEqualizationProcessorImpl.cpp \
		RasterGraylizeProcessorImpl.cpp \
		RasterReverseProcessorImpl.cpp \
		RasterSmoothProcessorImpl.cpp \
		RasterSubtractProcessorImpl.cpp \
		RasterPixelBlendProcessorImpl.cpp \
		RasterThresholdProcessorImpl.cpp \
		RasterFormatConvertToJPEGProcessorImpl.cpp \
		RasterSepiaToneEffecProcessorImpl.cpp \
		RasterPenEffectProcessorImpl.cpp \
		DemAspectProcessorImpl.cpp \
		DemSlopeProcessorImpl.cpp \
		DemHillshadeProcessorImpl.cpp \
		DemInundationProcessorImpl.cpp \
		GridPointGeneratorImpl.cpp \
		Delaunay.cpp \
		DelaunayProcessorImpl.cpp \
		IDWProcessorImpl.cpp \
		RasterThumbnailProcessorImpl.cpp \
		FeatureIDWProcessorImpl.cpp

