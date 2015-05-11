#include "WorkspaceMy.h"
#include "ConnectionMy.h"

namespace auge
{

	WorkspaceMy::WorkspaceMy()
	{

	}

	WorkspaceMy::~WorkspaceMy()
	{

	}

	const char*	WorkspaceMy::GetName()
	{
		return NULL;
	}

	void WorkspaceMy::SetName(const char* name)
	{

	}

	DataEngine*	WorkspaceMy::GetEngine()
	{
		return NULL;
	}

	RESULTCODE WorkspaceMy::SetConnectionString(const char* conn_string)
	{
		return m_myConnection.SetConnectionString(conn_string);
	}

	const char*	WorkspaceMy::GetConnectionString()
	{
		return m_myConnection.GetConnectionString();
	}

	RESULTCODE WorkspaceMy::Open()
	{
		if(m_myConnection.IsOpen())
		{
			return AG_SUCCESS;
		}
		RESULTCODE rc = m_myConnection.Open();
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}

		//if(!m_myConnection.HasTable(g_catalog_table.c_str()))
		//{
		//	rc = CreateCatalogTable();
		//}

		return rc;
	}

	void WorkspaceMy::Close()
	{

	}

	bool WorkspaceMy::IsOpen()
	{
		return true;
	}

	//EnumDataSet*	GetDataSets();
	DataSet* WorkspaceMy::OpenDataSet(const char* name)
	{
		return NULL;
	}

	RESULTCODE WorkspaceMy::RemoveDataSet(const char* name)
	{
		return AG_SUCCESS;
	}

	// FeatureClass
	EnumDataSet* WorkspaceMy::GetFeatureClasses()
	{
		return NULL;
	}

	FeatureClass* WorkspaceMy::OpenFeatureClass(const char* name)
	{
		return NULL;
	}

	FeatureClass* WorkspaceMy::CreateFeatureClass(const char* name, GFields* pFields)
	{
		return NULL;
	}

	RESULTCODE WorkspaceMy::RemoveFeatureClass(const char* name)
	{
		return AG_SUCCESS;
	}

	// Raster
	//void	WorkspaceMy::SetConnection(GConnection* pConnection)
	//{
	//	return NULL;
	//}

	EnumDataSet* WorkspaceMy::GetRasterDatasets()
	{
		return NULL;
	}

	RasterDataset* WorkspaceMy::OpenRasterDataset(const char* name)
	{
		return NULL;
	}

	RESULTCODE WorkspaceMy::AddRaster(Raster* pRaster)
	{
		return AG_SUCCESS;
	}

}