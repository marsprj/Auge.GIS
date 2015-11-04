AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgsauth.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgsauth_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Carto/src \
		-L$(top_srcdir)/Engine/Server/gscore/src

libgsauth_la_LIBADD=-lgcore -lgcarto -lgscore

libgsauth_la_SOURCES=\
		CreateUserHandler.cpp \
		CreateUserRequest.cpp \
		GetRoleHandler.cpp \
		GetRoleRequest.cpp \
		GetRoleResponse.cpp \
		GetUserHandler.cpp \
		GetUserRequest.cpp \
		GetUserResponse.cpp \
		GetCatalogHandler.cpp \
		GetCatalogResponse.cpp \
		GetCatalogRequest.cpp \
		RemoveUserHandler.cpp \
		RemoveUserRequest.cpp \
		LoginHandler.cpp \
		LoginRequest.cpp \
		LogoutHandler.cpp \
		LogoutRequest.cpp \
		IsLoginedHandler.cpp \
		IsLoginedRequest.cpp \
		GetLoginCountHandler.cpp \
		GetLoginCountRequest.cpp \
		GetLoginCountResponse.cpp \
		GetOnlineUserHandler.cpp \
		GetOnlineUserRequest.cpp \
		GetOnlineUserResponse.cpp \
		GetUserCountRequest.cpp \
		GetUserCountResponse.cpp \
		GetUserCountHandler.cpp \
		WAuthEngine.cpp 

