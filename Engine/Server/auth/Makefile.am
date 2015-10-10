AUTOMAKE_OPTION=foreign
bin_PROGRAMS=auth.cgi
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I$(top_srcdir)/include


auth_cgi_LDFLAGS= \
		-L$(top_srcdir)/Engine/Auth/src \
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Data/GDEngine/src \
		-L$(top_srcdir)/Engine/Carto/src \
		-L$(top_srcdir)/Engine/Server/gscore/src \
		-L$(top_srcdir)/Engine/Server/gsvrs/src \
		-L$(top_srcdir)/Engine/Server/gsims/src
	
auth_cgi_LDADD=	-lgcore \
		-lgauth \
      		-lgdengine \
  		-lgcarto \
		-lgscore \
		-lgsvrs \
		-lfcgi \
		-lrudecgi \
		-lgsims

auth_cgi_SOURCES=main.cpp GServer.cpp
