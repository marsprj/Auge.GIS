#ifndef __AUGE_WORKSPACE_MY_H__
#define __AUGE_WORKSPACE_MY_H__

#include "AugeFeature.h"
#include <string>
#include <map>

#include "ConnectionMy.h"

namespace auge
{
	class WorkspaceMy : virtual public FeatureWorksapce,
		virtual public RasterWorkspace
	{
		//friend class FeatureMy;
		//friend class FeatureClassMy;
		//friend class FeatureInsertCommandMy;

	public:
		WorkspaceMy();
		virtual ~WorkspaceMy();
	public:
		virtual const char*		GetName();
		virtual void			SetName(const char* name);
		virtual	DataEngine*		GetEngine();

	public:
		virtual	RESULTCODE		SetConnectionString(const char* conn_string);
		virtual const char*		GetConnectionString();

		virtual RESULTCODE		Open();
		virtual void			Close();
		virtual bool			IsOpen();

		//virtual EnumDataSet*	GetDataSets();
		virtual DataSet*		OpenDataSet(const char* name);
		virtual RESULTCODE		RemoveDataSet(const char* name);

		// FeatureClass
		virtual EnumDataSet*	GetFeatureClasses();
		virtual	FeatureClass*	OpenFeatureClass(const char* name);
		virtual FeatureClass*	CreateFeatureClass(const char* name, GFields* pFields);
		virtual RESULTCODE		RemoveFeatureClass(const char* name);

		// Raster
		//virtual void			SetConnection(GConnection* pConnection);
		virtual EnumDataSet*	GetRasterDatasets();
		virtual RasterDataset*	OpenRasterDataset(const char* name);
		virtual RESULTCODE		AddRaster(Raster* pRaster);

	private:
		ConnectionMy	m_myConnection;
		std::string		m_name;
		std::string		m_schema;

		std::string		g_catalog_table;
	};
}

#endif //__AUGE_WORKSPACE_MY_H__