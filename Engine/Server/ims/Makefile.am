AUTOMAKE_OPTION=foreign
bin_PROGRAMS=ims.cgi
AM_CXXFLAGS=-D_LINUX
AM_CPPFLAGS=\
	    	-I$(top_srcdir)/include	
ims_cgi_LDFLAGS= \
 		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Data/GDEngine/src \
		-L$(top_srcdir)/Engine/Carto/src \
		-L$(top_srcdir)/Engine/Service/WCore/src \
		-L$(top_srcdir)/Engine/Service/WService/src
	
ims_cgi_LDADD=	-lgcore \
		-lgdengine \
		-lgcarto \
		-lwcore \
		-lwservice \
		-lfcgi \
		-lrudecgi

ims_cgi_SOURCES=ims.cpp GServer.cpp

