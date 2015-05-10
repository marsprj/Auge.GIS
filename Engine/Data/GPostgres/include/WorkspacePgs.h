#ifndef __AUGE_WORKSPACE_PGS_H__
#define __AUGE_WORKSPACE_PGS_H__

#include "AugeFeature.h"
#include <string>
#include <map>

#include "ConnectionPgs.h"

namespace auge
{
	class WorkspacePgs : virtual public FeatureWorksapce,
						 virtual public RasterWorkspace
	{
		friend class FeaturePgs;
		friend class FeatureClassPgs;
		friend class FeatureInsertCommandPgs;

	public:
		WorkspacePgs();
		virtual ~WorkspacePgs();
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
		virtual RESULTCODE		CreateFeatureClass(const char* name, GFields* pFields);
		virtual RESULTCODE		RemoveFeatureClass(const char* name);

		// Raster
		//virtual void			SetConnection(GConnection* pConnection);
		virtual EnumDataSet*	GetRasterDatasets();
		virtual RasterDataset*	OpenRasterDataset(const char* name);
		virtual RESULTCODE		AddRaster(Raster* pRaster);

	private:
		// Table
		RESULTCODE				CreateTable(const char* name, GFields* pFields);
		RESULTCODE				RemoveTable(const char* name);

		// FeatureClass
		bool					UnRegisterLayer(long lid);
		//RESULTCODE				RegiseterGeometryColumns(const char* szName, AgField* pGeomField);
		RESULTCODE				AddGeometryColumn(const char* name, GField* pField);
		RESULTCODE				RegiseterGeometryColumn(const char* name, GField* pField);
		RESULTCODE				UnRegiseterGeometryColumn(const char* name);
		
		// Raster

		RESULTCODE				CreateCatalogTable();
	private:
		ConnectionPgs	m_pgConnection;
		std::string		m_name;
		std::string		m_schema;

		std::string		g_catalog_table;
	};
}

#endif //__AUGE_WORKSPACE_PGS_H__
