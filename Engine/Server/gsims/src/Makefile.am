AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgsims.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgsims_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Carto/src \
		-L$(top_srcdir)/Engine/Server/gscore/src \
		-L$(top_srcdir)/Engine/Server/gsvrs/src

libgsims_la_LIBADD=-lgcore -lgcarto -lgscore -lgsvrs

libgsims_la_SOURCES=\
		    CapabilitiesHandler.cpp \
		    CapabilitiesRequest.cpp \
		    CapabilitiesResponse.cpp \
		    CreateMapHandler.cpp \
		    CreateMapRequest.cpp \
		    CreateServiceHandler.cpp \
		    CreateServiceRequest.cpp \
		    CreateServiceResponse.cpp \
		    GetDataSourceHandler.cpp \
		    GetDataSourceRequest.cpp \
		    GetDataSourceResponse.cpp \
		    GetMapHandler.cpp \
		    GetMapRequest.cpp \
		    GetMapResponse.cpp \
		    GetServiceHandler.cpp \
		    GetServiceRequest.cpp \
		    GetServiceResponse.cpp \
		    GetStyleHandler.cpp \
		    GetStyleRequest.cpp \
		    GetStyleResponse.cpp \
		    RegisterDataSourceHandler.cpp \
		    RegisterDataSourceRequest.cpp \
		    RegisterLayerHandler.cpp \
		    RegisterLayerRequest.cpp \
		    RemoveMapHandler.cpp \
		    RemoveMapRequest.cpp \
		    RemoveServiceHandler.cpp \
		    RemoveServiceRequest.cpp \
		    RemoveServiceResponse.cpp \
		    SetStyleHandler.cpp \
		    SetStyleRequest.cpp \
		    AddStyleHandler.cpp \
		    AddStyleRequest.cpp \
		    RemoveStyleHandler.cpp \
		    RemoveStyleRequest.cpp \
		    UpdateStyleHandler.cpp \
		    UpdateStyleRequest.cpp \
		    UnRegisterDataSourceHandler.cpp \
		    UnRegisterDataSourceRequest.cpp \
		    RegisterMapHandler.cpp \
		    RegisterMapRequest.cpp \
		    GetFontHandler.cpp \
		    GetFontRequest.cpp \
		    GetFontResponse.cpp \
		    WEngine.cpp

