AUTOMAKE_OPTION=foreign
bin_PROGRAMS=pgs2pgs
AM_CXXFLAGS=-D_LINUX
AM_CPPFLAGS=\
	-I$(top_srcdir)/include	

pgs2pgs_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Data/GPostgres/src \
		-L$(top_srcdir)/Engine/Data/GDEngine/src \
		-L$(top_srcdir)/Engine/Raster/src \
		-L$(top_srcdir)/Engine/Filter/src 
	
pgs2pgs_LDADD=	-lgcore \
		-lGPostgres \
		-lgdengine \
		-lgfilter \
		-lgraster \
		-lcppunit

pgs2pgs_SOURCES=main.cpp
