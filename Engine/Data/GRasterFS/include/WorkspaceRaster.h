#ifndef __AUGE_RASTER_WORKSPACE__FS_H__
#define __AUGE_RASTER_WORKSPACE__FS_H__

#include "AugeData.h"
#include <string>
#include <map>

namespace auge
{
	class WorkspaceRaster : public RasterWorkspace
	{
	public:
		WorkspaceRaster();
		virtual ~WorkspaceRaster();
	public:
		// Base Methods
		virtual const char*		GetName();
		virtual void			SetName(const char* name);

		virtual	RESULTCODE		SetConnectionString(const char* conn_string);
		virtual const char*		GetConnectionString();

		virtual RESULTCODE		Open();
		virtual void			Close();
		virtual bool			IsOpen();

		virtual EnumDataSet*	GetDataSets();
		virtual DataSet*		OpenDataSet(const char* name);
		virtual RESULTCODE		RemoveDataSet(const char* name);

		virtual	DataEngine*		GetEngine();

		virtual void			Release();

		// Raster Methods
		virtual EnumDataSet*	GetRasterDatasets();
		virtual RasterDataset*	OpenRasterDataset(const char* name);

		virtual	Raster*			OpenRaster(const char* name);
		virtual RESULTCODE		AddRaster(Raster* pRaster);
		virtual RESULTCODE		RemoveRaster(const char* name);
	private:
		std::string	m_connection_string;
		std::string	m_name;
		std::string	m_path;
	};
}

#endif //__AUGE_RASTER_WORKSPACE__FS_H__
