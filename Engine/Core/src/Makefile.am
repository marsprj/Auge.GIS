AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgcore.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgcore_la_LDFLAGS=

libgcore_la_LIBADD=-lxml2 -llog4cplus -luuid

libgcore_la_SOURCES=\
		FieldFactoryImpl.cpp \
		GColor.cpp \
		GEnvelope.cpp \
		GeometryDefImpl.cpp \
		GeometryFactoryImpl.cpp \
		GeometryUtilImpl.cpp \
		GErrorImpl.cpp \
		GFieldImpl.cpp \
		GFieldsImpl.cpp \
		GFunction.cpp \
		GLineStringWKB.cpp \
		GLoggerImpl.cpp \
		GMultiLineStringWKB.cpp \
		GMultiPointWKB.cpp \
		GMultiPolygonWKB.cpp \
		GPointWKB.cpp \
		GPolygonWKB.cpp \
		GPropertySet.cpp \
		GValue.cpp \
		StringTokenizer.cpp \
		WKTReader.cpp \
		WKTWriter.cpp \
		XAttribute.cpp \
		XCdataNode.cpp \
		XCommentNode.cpp \
		XContentNode.cpp \
		XDocument.cpp \
		XDtd.cpp \
		XElement.cpp \
		XEntityReference.cpp \
		XNode.cpp \
		XNodeSet.cpp \
		XParser.cpp \
		XProcessingInstructionNode.cpp \
		XTestNode.cpp \
		GMLWriterImpl.cpp
