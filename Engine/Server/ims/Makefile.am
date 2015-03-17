AUTOMAKE_OPTION=foreign
bin_PROGRAMS=ims.cgi
AM_CXXFLAGS=-D_LINUX
AM_CPPFLAGS=\
	    	-I$(top_srcdir)/include	
ims_cgi_LDFLAGS= \
 		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Data/GDEngine/src \
		-L$(top_srcdir)/Engine/Carto/src \
		-L$(top_srcdir)/Engine/Server/gscore/src \
		-L$(top_srcdir)/Engine/Server/gsvrs/src
	
ims_cgi_LDADD=	-lgcore \
		-lgdengine \
		-lgcarto \
		-lgscore \
		-lgsvrs \
		-lfcgi \
		-lrudecgi

ims_cgi_SOURCES=ims.cpp GServer.cpp

