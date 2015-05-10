#ifndef __AUGE_RASTER_WORKSPACE_IMPL_H___
#define __AUGE_RASTER_WORKSPACE_IMPL_H___

#include "AugeRaster.h"
#include <string>
#include <map>

namespace auge
{
	class WorkspaceRasterDB : public RasterWorkspace
	{
	public:
		WorkspaceRasterDB();
		virtual ~WorkspaceRasterDB();
	public:
		// Base Methods
		virtual const char*		GetName();
		virtual void			SetName(const char* name);

		virtual	RESULTCODE		SetConnectionString(const char* conn_string);
		virtual const char*		GetConnectionString();
		virtual void			SetConnection(GConnection* pConnection);

		virtual RESULTCODE		Open();
		virtual void			Close();
		virtual bool			IsOpen();

		virtual EnumDataSet*	GetDataSets();
		virtual DataSet*		OpenDataSet(const char* name);

		virtual	DataEngine*		GetEngine();

		virtual void			Release();

		// Raster Methods
		virtual	Raster*			OpenRaster(const char* name);
		virtual RESULTCODE		AddRaster(Raster* pRaster);
		virtual RESULTCODE		RemoveRaster(g_uint id);

	private:
		RESULTCODE				CreateCatalogTable();
	private:
		GConnection *m_pCnnection;
		std::string	m_name;
		std::string	m_path;
		std::string g_catalog_table;
	};
}

#endif //__AUGE_RASTER_WORKSPACE_IMPL_H___
