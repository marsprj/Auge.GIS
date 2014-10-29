#include "Workspace.h"
#include "FeatureType.h"

namespace auge
{

	Connection::Connection():
	m_pgConnection(NULL)
	{

	}

	Connection::~Connection()
	{
		Close();
	}

	int	Connection::Open(const char* connstring)
	{
		if(connstring==NULL)
		{
			return AG_FAILURE;
		}

		if(m_pgConnection!=NULL)
		{
			return AG_FAILURE;
		}

		ConnStatusType status;
		m_pgConnection = PQconnectdb(connstring);
		status = PQstatus(m_pgConnection);
		if(status != CONNECTION_OK)
		{
			PQfinish(m_pgConnection);
			m_pgConnection = NULL;
			return AG_FAILURE;
		}
		m_connstring = connstring;
		return AG_SUCCESS;
	}

	int	Connection::Close()
	{
		if(m_pgConnection!=NULL)
		{
			return AG_SUCCESS;
		}
		PQfinish(m_pgConnection);
		m_pgConnection = NULL;
		m_connstring.clear();
		return AG_SUCCESS;
	}

	std::vector<FeatureType*>& Connection::GetFeatureTypes()
	{
		char sql[PATH_MAX] = {0};
		PGresult* pgResult = NULL;
		ExecStatusType status;

		Cleanup();
		
		sprintf(sql, "select f_table_name,f_geometry_column,coord_dimension,srid,type from geometry_columns");
		pgResult = PQexec(m_pgConnection, sql);
		status = PQresultStatus(pgResult);
		if(status != PGRES_TUPLES_OK)
		{
			PQclear(pgResult);
		}

		FeatureType *pType = NULL;
		char *name,*geom,*dimn,*srid,*type;
		int num = PQntuples(pgResult);
		for(int i=0; i<num; i++)
		{	
			name = PQgetvalue(pgResult, i, 0);	//f_table_name
			geom = PQgetvalue(pgResult, i, 1);	//f_geometry_column
			dimn = PQgetvalue(pgResult, i, 2);	//coord_dimension
			srid = PQgetvalue(pgResult, i, 3);	//srid
			type = PQgetvalue(pgResult, i, 4);	//type

			pType = new FeatureType(this);
			pType->SetName(name);
			pType->SetGeometryColumn(geom);
			pType->SetDimension(atoi(dimn));
			pType->SetSrid(atoi(srid));
			pType->SetGeometryType(type);

			m_types.push_back(pType);
		}

		PQclear(pgResult);

		return m_types;
	}

	FeatureType* Connection::OpenFeatureType(const char* szName)
	{
		char sql[PATH_MAX] = {0};
		PGresult* pgResult = NULL;
		ExecStatusType status;

		if(szName==NULL)
		{
			return NULL;
		}
		
		sprintf(sql, "select f_table_name,f_geometry_column,coord_dimension,srid,type from geometry_columns where f_table_name='%s'", szName);
		pgResult = PQexec(m_pgConnection, sql);
		status = PQresultStatus(pgResult);
		if(status != PGRES_TUPLES_OK)
		{
			PQclear(pgResult);
		}

		int num = PQntuples(pgResult);
		if(num!=1)
		{
			PQclear(pgResult);
			return NULL;
		}

		char *name,*geom,*dimn,*srid,*type;
		name = PQgetvalue(pgResult, 0, 0);	//f_table_name
		geom = PQgetvalue(pgResult, 0, 1);	//f_geometry_column
		dimn = PQgetvalue(pgResult, 0, 2);	//coord_dimension
		srid = PQgetvalue(pgResult, 0, 3);	//srid
		type = PQgetvalue(pgResult, 0, 4);	//type

		FeatureType *pType = new FeatureType(this);
		pType->SetName(name);
		pType->SetGeometryColumn(geom);
		pType->SetDimension(atoi(dimn));
		pType->SetSrid(atoi(srid));
		pType->SetGeometryType(type);
		PQclear(pgResult);

		return pType;
	}

	void Connection::Cleanup()
	{
		FeatureType* pType = NULL;
		std::vector<FeatureType*>::iterator iter;
		for(iter=m_types.begin(); iter!=m_types.end(); iter++)
		{
			pType = *iter;
			pType->Release();
		}
		m_types.clear();
	}
}
