AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgsims.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgsims_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Carto/src \
		-L$(top_srcdir)/Engine/Raster/src \
		-L$(top_srcdir)/Engine/Server/gscore/src \
		-L$(top_srcdir)/Engine/Server/gsvrs/src

libgsims_la_LIBADD=-lgcore -lgcarto -lgscore -lgsvrs -lgraster

libgsims_la_SOURCES=\
		    CapabilitiesHandler.cpp \
		    CapabilitiesRequest.cpp \
		    CapabilitiesResponse.cpp \
		    CreateMapHandler.cpp \
		    CreateMapRequest.cpp \
		    SaveMapHandler.cpp \
		    SaveMapRequest.cpp \
		    CreateServiceHandler.cpp \
		    CreateServiceRequest.cpp \
		    CreateServiceResponse.cpp \
		    RemoveServiceHandler.cpp \
		    RemoveServiceRequest.cpp \
		    RemoveServiceResponse.cpp \
		    DescribeServiceHandler.cpp \
		    DescribeServiceRequest.cpp \
		    DescribeServiceResponse.cpp \
    		    DescribeMapHandler.cpp \
		    DescribeMapRequest.cpp \
		    DescribeMapResponse.cpp \
		    DescribeLayerHandler.cpp \
		    DescribeLayerRequest.cpp \
		    DescribeLayerResponse.cpp \
		    GetStyleHandler.cpp \
		    GetStyleRequest.cpp \
		    GetStyleResponse.cpp \
		    RegisterLayerHandler.cpp \
		    RegisterLayerRequest.cpp \
		    UnRegisterLayerRequest.cpp \
   		    RegisterLayerResponse.cpp \
    		    UnRegisterLayerHandler.cpp \
		    RemoveMapHandler.cpp \
		    RemoveMapRequest.cpp \
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
		    GetSymbolHandler.cpp \
		    GetSymbolRequest.cpp \
		    GetSymbolResponse.cpp \
		    GetSymbolIconHandler.cpp \
		    GetSymbolIconRequest.cpp \
		    GetSymbolIconResponse.cpp \
		    GetColorMapHandler.cpp \
		    GetColorMapResponse.cpp \
		    GetColorMapRequest.cpp \
		    UpdateMapLayerOrderHandler.cpp \
		    UpdateMapLayerOrderRequest.cpp \
		    GetMapScaleHandler.cpp \
		    GetMapScaleRequest.cpp \
		    GetMapScaleResponse.cpp \
		    SubscribeHandler.cpp \
		    SubscribeRequest.cpp \
		    GetSubscriptionHandler.cpp \
		    GetSubscriptionRequest.cpp \
		    GetSubscriptionHandler.cpp \
		    GetSubscriptionRequest.cpp \
		    GetSubscriptionResponse.cpp \
		    GetSubscriptionThemeHandler.cpp \
		    GetSubscriptionThemeRequest.cpp \
		    GetSubscriptionThemeResponse.cpp \
		    WEngine.cpp

