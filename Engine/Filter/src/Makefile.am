AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgfilter.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgfilter_la_LDFLAGS=-L$(top_srcdir)/Engine/Core/src

libgfilter_la_LIBADD=-lgcore

libgfilter_la_SOURCES=\
		ArithmeticExpressionImpl.cpp \
		BBoxFilterImpl.cpp \
		BinaryComparisonFilterImpl.cpp \
		BinaryLogicFilterImpl.cpp \
		BinarySpatialFilterImpl.cpp \
		DistanceBufferFilterImpl.cpp \
		EnumExpressionImpl.cpp \
		EnumFilterImpl.cpp \
		FilterFactoryImpl.cpp \
		FilterReaderImpl.cpp \
		FunctionImpl.cpp \
		IDFilterImpl.cpp \
		IsBetweenFilterImpl.cpp \
		IsLikeFilterImpl.cpp \
		IsNullFilterImpl.cpp \
		LiteralImpl.cpp \
		OrderByImpl.cpp \
		PropertyNameImpl.cpp \
		QueryImpl.cpp \
		UnaryLogicFilterImpl.cpp
