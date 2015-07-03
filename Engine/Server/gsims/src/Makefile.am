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
		    DescribeMapHandler.cpp \
		    DescribeMapRequest.cpp \
		    DescribeMapResponse.cpp \
		    GetServiceHandler.cpp \
		    GetServiceRequest.cpp \
		    GetServiceResponse.cpp \
		    GetStyleHandler.cpp \
		    GetStyleRequest.cpp \
		    GetStyleResponse.cpp \
		    RegisterLayerHandler.cpp \
		    RegisterLayerRequest.cpp \
		    UnRegisterLayerRequest.cpp \
    		    UnRegisterLayerHandler.cpp \
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
		    RegisterMapHandler.cpp \
		    RegisterMapRequest.cpp \
		    GetFontHandler.cpp \
		    GetFontRequest.cpp \
		    GetFontResponse.cpp \
		    GetColorMapHandler.cpp \
		    GetColorMapResponse.cpp \
		    GetColorMapRequest.cpp \
		    UpdateMapLayerOrderHandler.cpp \
		    UpdateMapLayerOrderRequest.cpp \
		    WEngine.cpp

