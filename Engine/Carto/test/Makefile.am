AUTOMAKE_OPTION=foreign
bin_PROGRAMS=cartotest
AM_CXXFLAGS=-D_LINUX
AM_CPPFLAGS=\
	-I$(top_srcdir)/include	

cartotest_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Carto/src \
		-L$(top_srcdir)/Engine/Data/GDEngine/src \
		-L$(top_srcdir)/Engine/Filter/src 
	
cartotest_LDADD=-lgcore \
		-lgdengine \
		-lgcarto \
		-lgfilter \
		-lcppunit

cartotest_SOURCES=CanvasTest.cpp main.cpp MapTest.cpp StyleTest.cpp
