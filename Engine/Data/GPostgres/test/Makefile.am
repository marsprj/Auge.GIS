AUTOMAKE_OPTION=foreign
bin_PROGRAMS=pgstest
AM_CXXFLAGS=-D_LINUX
AM_CPPFLAGS=\
	-I$(top_srcdir)/include	\
	-I$(PGHOME)/include \
	-I$(PROJ_HOME)/include	

pgstest_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Data/GPostgres/src \
		-L$(top_srcdir)/Engine/Data/GDEngine/src \
		-L$(top_srcdir)/Engine/Raster/src \
		-L$(top_srcdir)/Engine/Filter/src \
		-L$(PGHOME)/lib \
		-L$(PROJ_HOME)/lib
	
pgstest_LDADD=	-lgcore \
		-lGPostgres \
		-lgdengine \
		-lgfilter \
		-lgraster \
		-lcppunit \
		-lpq \
		-lproj

pgstest_SOURCES=main.cpp OgcFilterTest.cpp PgsTest.cpp Transfer.cpp PgRasterTest.cpp
