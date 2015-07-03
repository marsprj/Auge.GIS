#ifndef __AUGE_DATA_H__
#define __AUGE_DATA_H__

#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeGeometry.h"

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
	class GField;
	class GFields;
	class Raster;
	class DataEngine;
	class Workspace;
	class DataSet;
	class EnumDataSet;
	class GConnection;
	class GResultSet;
	class ConnectionManager;

	typedef enum
	{
		augeDataSetAttribute	= 0,
		augeDataSetFeature,
		augeDataSetRaster,
		augeDataSetTile
	}augeDataSetType;

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
		
		virtual EnumDataSet*	GetDataSets() = 0;
		virtual DataSet*		OpenDataSet(const char* name) = 0;
		virtual RESULTCODE		RemoveDataSet(const char* name) = 0;

		virtual	DataEngine*		GetEngine() = 0;
	};

	class EnumWorkspace
	{
	protected:
		EnumWorkspace(){}
		virtual ~EnumWorkspace(){}
	public:
		virtual void			Reset() = 0;
		virtual	Workspace*		Next() = 0;
		virtual void			Release() = 0;
	};

	class Row : public GObject
	{
	protected:
		Row(){}
		virtual ~Row(){}
	public:
		//virtual GValue*				GetValue(g_uint i)		const = 0;
		virtual bool				GetBool(g_uint i)		const = 0;
		virtual char				GetChar(g_uint i)		const = 0;
		virtual short				GetShort(g_uint i)		const = 0;
		virtual int					GetInt(g_uint i)		const = 0;
		virtual long				GetLong(g_uint i)		const = 0;
		virtual float				GetFloat(g_uint i)		const = 0;
		virtual double				GetDouble(g_uint i)		const = 0;
		virtual int64				GetInt64(g_uint i)		const = 0;
		virtual const char*			GetString(g_uint i)		const = 0;
		//virtual const AgString*	GetString(g_uint i)		const = 0;
		//virtual const AgBlob*	GetBlob(g_uint i)		const = 0;
		//virtual const TIME_STRU*GetTime(g_uint i)		const = 0;

		//virtual GValue*				GetValue(const char* name)		const = 0;
		virtual bool				GetBool(const char* name)		const = 0;
		virtual char				GetChar(const char* name)		const = 0;
		virtual short				GetShort(const char* name)		const = 0;
		virtual int					GetInt(const char* name)		const = 0;
		virtual long				GetLong(const char* name)		const = 0;
		virtual float				GetFloat(const char* name)		const = 0;
		virtual double				GetDouble(const char* name)		const = 0;
		virtual int64				GetInt64(const char* name)		const = 0;
		virtual const char*			GetString(const char* name)		const = 0;
	};

	class Cursor : public GObject
	{
	protected:
		Cursor(){}
		virtual ~Cursor(){}
	public:
		virtual Row*				NextRow() = 0;
		virtual DataSet*			GetDataSet() = 0;
	};

	class DataSet : public GObject
	{
	protected:
		DataSet(){}
		virtual ~DataSet(){}
	public:
		virtual const char*			GetName() = 0;
		virtual augeDataSetType		GetType() = 0;
	};

	class AttributeDataSet : public DataSet
	{
	protected:
		AttributeDataSet(){}
		virtual ~AttributeDataSet(){}
	public:
		virtual GFields*			GetFields() = 0;
		virtual GField*				GetField(const char* name) = 0;
		virtual Cursor*				GetRows() = 0;
	};

	class EnumDataSet : public GObject
	{
	protected:
		EnumDataSet(){}
		virtual ~EnumDataSet(){}
	public:
		virtual void				Reset() = 0;
		virtual DataSet*			Next() = 0;
		virtual bool				Add(DataSet* pDataSet) = 0;
		virtual void				Release() = 0;
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
		virtual EnumWorkspace*		GetWorkspaces() = 0;

		virtual Workspace*			NewWorkspace(const char* name) = 0;

		virtual RESULTCODE			Register(const char* name, const char* engine, const char* constr) = 0;
		virtual RESULTCODE			Unregister(const char* name) = 0;
		//virtual RESULTCODE			Update(const char* name, const char* engine, const char* constr) = 0;
		//virtual RESULTCODE			Update(g_uint gid, const char* name, const char* engine, const char* constr) = 0;

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

	class RasterDataset : public DataSet
	{
	public:
		RasterDataset(){}
		virtual ~RasterDataset(){}
	public:
		virtual const char*		GetName() = 0;
		virtual const char*		GetAlias() = 0;
		virtual g_uint			GetWidth() = 0;
		virtual g_uint			GetHeight() = 0;
		virtual g_uint			GetBandCount() = 0;
		virtual GEnvelope&		GetExtent() = 0;
		virtual g_int			GetSRID() = 0;
		virtual const char*		GetFormat() = 0;
		virtual const char*		GetPath() = 0;

		virtual Raster*			GetRaster() = 0;
	};

	class RasterWorkspace : virtual public Workspace
	{
	protected:
		RasterWorkspace(){}
		virtual ~RasterWorkspace(){}
	public:
		//virtual void			SetConnection(GConnection* pConnection) = 0;

		virtual const char*		GetRepository() = 0;

		virtual EnumDataSet*	GetRasterDatasets() = 0;
		virtual RasterDataset*	OpenRasterDataset(const char* name) = 0;
		//virtual RESULTCODE		RemoverRasterDataset(const char* name) = 0;

		//virtual	Raster*			OpenRaster(const char* name) = 0;
		virtual RESULTCODE		AddRaster(Raster* pRaster) = 0;
		virtual RESULTCODE		RemoveRaster(const char* name) = 0;
		//virtual RESULTCODE		RemoveRaster(g_uint id) = 0;
	};

	extern "C"
	{
		AUGE_DATA_ENGINE_API	DataEngineManager*	augeGetDataEngineManagerInstance();
		AUGE_DATA_ENGINE_API	ConnectionManager*	augeGetConnectionManagerInstance();
		AUGE_DATA_ENGINE_API	DataEngine*			augeGetDataEngineInstance();
		AUGE_DATA_ENGINE_API	const char*			augeGetDataSetType(augeDataSetType type);
	}
}

//==============================================================================
//------------------------------------------------------------------------------
// 连接所需动态库
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
