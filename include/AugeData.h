#ifndef __AUGE_DATA_H__
#define __AUGE_DATA_H__

#include "AugeData.h"

#ifdef WIN32
#	ifdef AUGE_DATA_ENGINE_EXPORTS
#		define AUGE_DATA_ENGINE_API __declspec(dllexport)
#	else
#		define AUGE_DATA_ENGINE_API __declspec(dllimport)
#	endif
#else
#	define AUGE_DATA_ENGINE_API
#endif

#include "AugeCore.h"

namespace auge
{
	class DataEngine;
	class Workspace;
	class GConnection;
	class GResultSet;
	class ConnectionManager;

	class GConnection : public GObject
	{
	protected:
		GConnection(){}
		virtual ~GConnection(){}
	public:
		virtual	RESULTCODE		SetConnectionString(const char* conn_string) = 0;
		virtual const char*		GetConnectionString() = 0;

		virtual RESULTCODE		Open() = 0;
		virtual void			Close() = 0;
		virtual bool			IsOpen() = 0;

		virtual GResultSet*		ExecuteQuery(const char* sql) = 0;
		virtual RESULTCODE		ExecuteSQL(const char* sql) = 0;

		virtual bool			HasTable(const char* name) = 0;
	};

	class GResultSet : public GObject
	{
	protected:
		GResultSet(){}
		virtual ~GResultSet(){}
	public:
		virtual	g_uint			GetCount() = 0;
		virtual g_int			GetInt(g_uint row, g_uint col) = 0;
		virtual double			GetDouble(g_uint row, g_uint col) = 0;
		virtual const char*		GetString(g_uint row, g_uint col) = 0;
	};

	class Workspace : public GObject
	{
	protected:
		Workspace(){}
		virtual ~Workspace(){}
	public:
		virtual const char*		GetName() = 0;
		virtual void			SetName(const char* name) = 0;

		virtual	RESULTCODE		SetConnectionString(const char* conn_string) = 0;
		virtual const char*		GetConnectionString() = 0;
		virtual RESULTCODE		Open() = 0;
		virtual void			Close() = 0;
		virtual bool			IsOpen() = 0;

		virtual	DataEngine*		GetEngine() = 0;
	};

	class DataEngine
	{
	protected:
		DataEngine(){}
		virtual ~DataEngine(){}
	public:
		virtual const char*			GetID() = 0;
		virtual const char*			GetDescription() = 0;

		virtual void*				GetHandler() = 0;
		virtual void				SetHandler(void* handler) = 0;

		virtual const char*			GetLibraryPath() = 0;

		virtual GConnection*		CreateConnection() = 0;
		virtual Workspace*			CreateWorkspace() = 0;
	};
	
	class ConnectionManager : public GObject
	{
	protected:
		ConnectionManager(){}
		virtual ~ConnectionManager(){}
	public:
		virtual g_int				GetCount() = 0;
		virtual Workspace*			GetWorkspace(g_uint i) = 0;
		virtual Workspace*			GetWorkspace(const char* name) = 0;
		virtual Workspace*			GetWorkspaceById(g_uint id) = 0;
		virtual RESULTCODE			Register(const char* name, const char* engine, const char* constr) = 0;
		virtual RESULTCODE			Unregister(const char* name) = 0;

		virtual bool				Has(const char* name) = 0;
		virtual g_int				GetID(const char* name) = 0;

		virtual RESULTCODE			Initialize(GConnection* pConnection) = 0;
		virtual void				Unload() = 0;
		
	};
	
	class DataEngineManager
	{
	protected:
		DataEngineManager(){}
		virtual ~DataEngineManager(){}

	public:
		virtual g_uint				GetCount() = 0;
		virtual DataEngine*			GetEngine(g_uint i) = 0;
		virtual DataEngine*			GetEngine(const char* id) = 0;
		virtual DataEngine*			Register(const char* path) = 0;
		virtual RESULTCODE			Unregister(const char* id) = 0;

		virtual	RESULTCODE			Load(const char* config_path=NULL) = 0;
		virtual void				Unload() = 0;
	};

	extern "C"
	{
		AUGE_DATA_ENGINE_API	DataEngineManager*	augeGetDataEngineManagerInstance();
		AUGE_DATA_ENGINE_API	ConnectionManager*	augeGetConnectionManagerInstance();
		AUGE_DATA_ENGINE_API	DataEngine*			augeGetDataEngineInstance(); 
	}
}

//==============================================================================
//------------------------------------------------------------------------------
// �������趯̬��
//------------------------------------------------------------------------------
#ifdef WIN32
#	ifndef AUGE_DATA_ENGINE_EXPORTS
#		define _AUGE_DATA_ENGINE_NAME "GDEngine"
#		if defined(_DEBUG)
#			if defined(_UNICODE)
//#				define _AUGE_FILTER_CONFIGURATION "UD"
#				define _AUGE_FILTER_CONFIGURATION "D"
#			else
#				define _AUGE_FILTER_CONFIGURATION "D"
#			endif // _UNICODE
#		else
#			if defined(_UNICODE)
#				define _AUGE_FILTER_CONFIGURATION "U"
#			else
#				define _AUGE_FILTER_CONFIGURATION ""
#			endif // _UNICODE
#		endif // _DEBUG
#		pragma comment(lib, _AUGE_DATA_ENGINE_NAME _AUGE_FILTER_CONFIGURATION ".lib")
#		pragma message("Automatically linking with " _AUGE_DATA_ENGINE_NAME _AUGE_FILTER_CONFIGURATION ".dll")
#		undef _AUGE_FILTER_CONFIGURATION
#		undef _AUGE_DATA_ENGINE_NAME
#	endif // AUGE_FILTER_EXPORTS 
#endif
//==============================================================================

#endif //__AUGE_DATA_H__
