AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgcarto.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgcarto_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Filter/src

libgcarto_la_LIBADD=-lgcore -lgfilter -lcairo

libgcarto_la_SOURCES=\
		CanvasImpl.cpp \
		CartoFactoryImpl.cpp \
		CartoManagerImpl.cpp \
		ColorMapImp.cpp \
		ColorMapImpl.cpp \
		FeatureLayerImpl.cpp \
		GraphicLayerImpl.cpp \
		FeatureStyleImpl.cpp \
		FillImpl.cpp \
		FontImpl.cpp \
		LineSymbolizerImpl.cpp \
		MapImpl.cpp \
		MapManager.cpp \
		MarkImpl.cpp \
		PointSymbolizerImpl.cpp \
		PolygonSymbolizerImpl.cpp \
		RasterLayerImpl.cpp \
		RasterStyleImpl.cpp \
		RasterSymbolizerImpl.cpp \
		RendererCairo.cpp \
		RuleImpl.cpp \
		StrokeImpl.cpp \
		StyleFactoryImpl.cpp \
		StyleReader_1_0_0.cpp \
		StyleReaderImpl.cpp \
		StyleWriter_1_0_0.cpp \
		StyleWriterImpl.cpp \
		TextSymbolizerImpl.cpp \
		TransformImpl.cpp \
		EnumStyleImpl.cpp \
		PointLabel.cpp \
		LineLabel.cpp \
		PolygonLabel.cpp \
		Label.cpp \
		LabelSet.cpp \
		Maplex.cpp \
		EnumMapImpl.cpp \
		ExternalGraphicImpl.cpp \
		EnumColorMapImpl.cpp \
		QuadServerLayerImpl.cpp \
		GraphicImpl.cpp \
		EnumSymbolImpl.cpp \
		SymbolManagerImpl.cpp \
		CapitalMarkerImpl.cpp \
		CircleMarkerImpl.cpp \
		PentagonMarkerImpl.cpp \
		StarMarkerImpl.cpp \
		SquareMarkerImpl.cpp \
		TriangleMarkerImpl.cpp \
		SimpleLineSymbolImpl.cpp \
		RailwaySymbolImpl.cpp \
		GrassLandSymbolImpl.cpp \
		SimpleRegionSymbolImpl.cpp
