AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgsgps.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include \
		-I$(GEOS_HOME)/include

libgsgps_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Processor/src \
		-L$(top_srcdir)/Engine/Server/gscore/src \
		-L$(GEOS_HOME)/lib

libgsgps_la_LIBADD=-lgcore -lgscore -lgprocessor -lgeos_c

libgsgps_la_SOURCES=\
		threadpool.c \
		BuildPyramidHandler.cpp \
		BuildPyramidRequest.cpp \
		CapabilitiesHandler.cpp \
		CapabilitiesRequest.cpp \
		CapabilitiesResponse.cpp \
		CentroidHandler.cpp \
		CentroidRequest.cpp \
		CentroidResponse.cpp \
		BufferHandler.cpp \
		BufferRequest.cpp \
		BufferResponse.cpp \
		ThreadManager.cpp \
		TileUpdateHandler.cpp \
		TileUpdateRequest.cpp \
		FeatureImportRequest.cpp \
		FeatureImportHandler.cpp \
		FeatureProjectHandler.cpp \
		FeatureProjectRequest.cpp \
		RasterExtractByRectangleHandler.cpp \
		RasterExtractByRectangleRequest.cpp \
		ClusterKmeanHandler.cpp \
		ClusterKmeanRequest.cpp \
		RasterEdgeDetectHandler.cpp \
		RasterEdgeDetectRequest.cpp \
		RasterReverseHandler.cpp \
		RasterReverseRequest.cpp \
		RasterGraylizeHandler.cpp \
		RasterGraylizeRequest.cpp \
		RasterSmoothHandler.cpp \
		RasterSmoothRequest.cpp \
		RasterStretchHandler.cpp \
		RasterStretchRequest.cpp \
		RasterSubtractHandler.cpp \
		RasterSubtractRequest.cpp \
		RasterSepiaToneHandler.cpp \
		RasterSepiaToneRequest.cpp \
		RasterPixelBlendHandler.cpp \
		RasterPixelBlendRequest.cpp \
		RasterThresholdHandler.cpp \
		RasterThresholdRequest.cpp \
		RasterHistogramEqualizationHandler.cpp \
		RasterHistogramEqualizationRequest.cpp \
		DemSlopeHandler.cpp \
		DemSlopeRequest.cpp \
		DemAspectHandler.cpp \
		DemAspectRequest.cpp \
		ConvexHullHandler.cpp \
		ConvexHullRequest.cpp \
		ConvexHullResponse.cpp \
		GetAreaHandler.cpp \
		GetAreaResponse.cpp \
		GetAreaRequest.cpp \
		GetLengthHandler.cpp \
		GetLengthResponse.cpp \
		GetLengthRequest.cpp \
		LineToPointsHandler.cpp \
		LineToPointsRequest.cpp \
		MultiPointToPointsHandler.cpp \
		MultiPointToPointsRequest.cpp \
		PolygonToPointsHandler.cpp \
		PolygonToPointsRequest.cpp \
		PolygonToLineHandler.cpp \
		PolygonToLineRequest.cpp \
		GenerateRandomPointsHandler.cpp \
		GenerateRandomPointsRequest.cpp \
		GenerateRandomPointsInPolygonHandler.cpp \
		GenerateRandomPointsInPolygonRequest.cpp \
		GetSpatialReferenceHandler.cpp \
		GetSpatialReferenceRequest.cpp \
		GetSpatialReferenceResponse.cpp \
		GetSpatialReferenceCountHandler.cpp \
		GetSpatialReferenceCountRequest.cpp \
		GetSpatialReferenceCountResponse.cpp \
		sqlite3.c \
		GProcessEngine.cpp

