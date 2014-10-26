AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libmapbrush.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I$(PGHOME)/include

libmapbrush_la_LDFLAGS=-L$(PGHOME)/lib

libmapbrush_la_LIBADD=-lpq

libmapbrush_la_SOURCES= Canvas.cpp \
			Feature.cpp \
			FeatureType.cpp \
			Layer.cpp \
			MapManager.cpp \
			RasterLayer.cpp \
			Style.cpp \
			Transformation.cpp \
			Workspace.cpp \
			Connection.cpp \
			FeatureLayer.cpp \
			Field.cpp \
			LineSymbolizer.cpp \
			PointSymbolizer.cpp \
			Renderer.cpp \
			Symbolizer.cpp 
			Util.cpp \
			Envelope.cpp \
			FeatureSet.cpp \
		   	Filter.cpp \
			Map.cpp \
			PolygonSymbolizer.cpp \
			Rule.cpp \
			TextSymbolizer.cpp\
			Viewer.cpp

