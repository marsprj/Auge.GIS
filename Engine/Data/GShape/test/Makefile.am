AUTOMAKE_OPTION=foreign
bin_PROGRAMS=shptest
AM_CXXFLAGS=-D_LINUX
AM_CPPFLAGS=\
	-I$(top_srcdir)/include	

shptest_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Data/GShape/src
	
shptest_LDADD=	-lgcore \
		-lGShape \
		-lcppunit

shptest_SOURCES=main.cpp ShapeTest.cpp
