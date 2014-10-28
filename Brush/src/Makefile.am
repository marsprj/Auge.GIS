AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libmapbrush.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I$(PGHOME)/include

libmapbrush_la_LDFLAGS=-L$(PGHOME)/lib

libmapbrush_la_LIBADD=-lpq \
		      -lxml2

libmapbrush_la_SOURCES= Workspace.cpp \
			Envelope.cpp \
			Field.cpp \
			Feature.cpp \
			FeatureSet.cpp \
			FeatureType.cpp \
			Filter.cpp \
			Map.cpp \
			Layer.cpp \
			RasterLayer.cpp \
			FeatureLayer.cpp \
			Symbolizer.cpp \
			PointSymbolizer.cpp \
			LineSymbolizer.cpp \
			PolygonSymbolizer.cpp \
			TextSymbolizer.cpp \
			Style.cpp \
			Rule.cpp \
			Canvas.cpp \
			Viewer.cpp \
			Transformation.cpp \
			Renderer.cpp \
			MapIO.cpp \
			StyleIO.cpp \
			Util.cpp

