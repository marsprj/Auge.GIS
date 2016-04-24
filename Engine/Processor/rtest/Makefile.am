AUTOMAKE_OPTION=foreign
bin_PROGRAMS=rtest
AM_CXXFLAGS=-D_LINUX
AM_CPPFLAGS=\
	-I$(top_srcdir)/include	

rtest_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Data/GDEngine/src \
		-L$(top_srcdir)/Engine/Processor/src \
		-L$(top_srcdir)/Engine/Carto/src 
	
rtest_LDADD=	-lgcore \
		-lgcarto \
		-lgprocessor \
		-lgdengine

rtest_SOURCES=main.cpp
