#include "FeatureType.h"
#include "FeatureSet.h"
#include "Field.h"
#include "Workspace.h"
#include "Util.h"

namespace auge
{

	FeatureType::FeatureType(Connection* pWorkspace):
	m_pWorkspace(pWorkspace)
	{

	}

	FeatureType::~FeatureType()
	{

	}

	const char*	FeatureType::GetName()
	{
		return m_name.c_str();
	}

	Envelope& FeatureType::GetExtent()
	{
		if(m_extent.IsValid())
		{
			return m_extent;
		}

		char sql[PATH_MAX] = {0};
		PGresult* pgResult = NULL;
		ExecStatusType status;

		int gindex = GetGeometryFieldIndex();
		Field& f = GetField(gindex);
		float xmin,ymin,xmax,ymax;

		ag_snprintf(sql, PATH_MAX, "select st_extent(\"%s\") from \"%s\"", f.GetName(), m_name.c_str());
		pgResult = PQexec(m_pWorkspace->m_pgConnection, sql);
		status = PQresultStatus(pgResult);
		if(status==PGRES_TUPLES_OK)
		{
			char* str = PQgetvalue(pgResult, 0, 0);
			sscanf(str, "BOX(%f %f,%f %f)", &xmin, &ymin,&xmax, &ymax);
			m_extent.m_xmin = xmin;
			m_extent.m_ymin = ymin;
			m_extent.m_xmax = xmax;
			m_extent.m_ymax = ymax;
		}

		PQclear(pgResult);

		return m_extent;
	}

	int	FeatureType::SetName(const char* szName)
	{
		if(szName==NULL)
		{
			return AG_FAILURE;
		}
		m_name = szName;

		return AG_SUCCESS;
	}

	int	FeatureType::SetGeometryColumn(const char* szGeometryColumn)
	{
		if(szGeometryColumn==NULL)
		{
			return AG_FAILURE;
		}
		m_geometry_column = szGeometryColumn;

		return AG_SUCCESS;
	}

	int	FeatureType::SetGeometryType(const char* szType)
	{
		if(szType==NULL)
		{
			return AG_FAILURE;
		}
		m_geom_type = Util::ParseGeometryType(szType);
		return AG_SUCCESS;
	}

	void FeatureType::SetDimension(int dimension)
	{
		m_dimension = dimension;
	}

	void FeatureType::SetSrid(int srid)
	{
		m_srid = srid;
	}

	Field& FeatureType::GetField(uint i)
	{
		std::vector<Field*> fields = GetFields();
		return *(fields[i]);
	}

	std::vector<Field*>& FeatureType::GetFields()
	{
		if(!m_fields.empty())
		{
			return m_fields;
		}

		char sql[PATH_MAX] = {0};
		PGresult* pgResult = NULL;
		ExecStatusType status;

		CleanupFields();

		sprintf(sql, "select * from %s limit 0", m_name.c_str());
		pgResult = PQexec(m_pWorkspace->m_pgConnection, sql);
		status = PQresultStatus(pgResult);
		if(status!=PGRES_TUPLES_OK)
		{
			PQclear(pgResult);
			return m_fields;
		}

		Field* f = NULL;
		char* name = NULL;
		Oid	  type;
		int	  size = 0;
		GeometryType gtype;
		int nfields = PQnfields(pgResult);
		for(int i=0; i<nfields; i++)
		{
			name = PQfname(pgResult, i);
			type = PQftype(pgResult, i);
			size = PQfsize(pgResult, i);

			f = new Field();
			f->SetName(name);
			f->SetType(Util::ParseFieldType(type));
			f->SetLength(size);

			if(f->GetType()==augeFieldTypeGeometry)
			{
				f->SetGeometryType(m_geom_type);
			}

			m_fields.push_back(f);
		}

		return m_fields;
	}

	int	FeatureType::GetFieldIndex(const char* szName)
	{
		Field* f = NULL;
		FieldType type = augeFieldTypeNone;
		std::vector<Field*> fields = GetFields();
		std::vector<Field*>::iterator iter;
		for(iter=m_fields.begin(); iter!=m_fields.end(); iter++)
		{
			f = *iter;
			if(strcmp(f->GetName(), szName)==0)
			{
				return (iter-m_fields.begin());
			}
		}
		return -1;
	}

	int	FeatureType::GetGeometryFieldIndex()
	{
		Field* f = NULL;
		FieldType type = augeFieldTypeNone;
		std::vector<Field*> fields = GetFields();
		std::vector<Field*>::iterator iter;
		for(iter=m_fields.begin(); iter!=m_fields.end(); iter++)
		{
			f = *iter;
			if(f->GetType()==augeFieldTypeGeometry)
			{
				return (iter-m_fields.begin());
			}
		}
		return -1;
	}

	FeatureSet*	FeatureType::Query(GeometryFormat format/*=WKB*/)
	{
		char sql[PATH_MAX] = {0};
		FeatureSet	*pFeatureSet = NULL;
		PGresult	*pgResult = NULL;
		ExecStatusType status;

		BuildQuerySQL(sql,PATH_MAX, format);

		pgResult = PQexec(m_pWorkspace->m_pgConnection, sql);
		status = PQresultStatus(pgResult);
		if(status!=PGRES_TUPLES_OK)
		{
			char* msg = PQerrorMessage(m_pWorkspace->m_pgConnection);
			PQclear(pgResult);
			return NULL;
		}

		pFeatureSet = new FeatureSet();
		pFeatureSet->Create(this, pgResult, format);
		
		return pFeatureSet;
	}

	FeatureSet*	FeatureType::Query(Envelope& extent, GeometryFormat format/*=WKB*/)
	{
		char sql[PATH_MAX] = {0};
		FeatureSet	*pFeatureSet = NULL;
		PGresult	*pgResult = NULL;
		ExecStatusType status;

		BuildQuerySQL(sql,PATH_MAX, extent, format);

		pgResult = PQexec(m_pWorkspace->m_pgConnection, sql);
		status = PQresultStatus(pgResult);
		if(status!=PGRES_TUPLES_OK)
		{
			char* msg = PQerrorMessage(m_pWorkspace->m_pgConnection);
			PQclear(pgResult);
			return NULL;
		}

		pFeatureSet = new FeatureSet();
		pFeatureSet->Create(this, pgResult, format);

		return pFeatureSet;
	}

	void FeatureType::CleanupFields()
	{
		Field* f = NULL;
		std::vector<Field*>::iterator iter;
		for(iter=m_fields.begin(); iter!=m_fields.end(); iter++)
		{
			f = *iter;
			if(f!=NULL)
			{
				f->Release();
			}
		}
		m_fields.clear();
	}

	int FeatureType::BuildQuerySQL(char* sql, uint size, GeometryFormat format/*=WKB*/)
	{	
		bool b = true;
		Field *f = NULL;
		const char* fname = NULL;		
		std::vector<Field*>::iterator iter;
		
		std::vector<Field*>& fields = GetFields();
		ag_snprintf(sql, size, "select ");
		for(iter=fields.begin(); iter!=fields.end(); iter++)
		{
			if(b)
				b = false;
			else
				 strcat(sql, ",");

			f = *iter;
			fname = f->GetName();
			if(f->GetType()==augeFieldTypeGeometry)
			{
				switch(format)
				{
				case WKT:
					strcat(sql, "st_astext(");
					break;
				case WKB:
					strcat(sql, "st_asbinary(");
					break;
				case GML2:
					strcat(sql, "st_asgml(");
					break;
				//case GML3:
				//	break;
				}
				strcat(sql, "\"");
				strcat(sql, fname);
				strcat(sql, "\"");
				strcat(sql, ") as ");				
				strcat(sql, "\"");
				strcat(sql, fname);
				strcat(sql, "\"");
			}
			else
			{
				strcat(sql, "\"");
				strcat(sql, fname);
				strcat(sql, "\"");
			}
		}
		strcat(sql, " from ");
		strcat(sql, "\"");
		strcat(sql, m_name.c_str());
		strcat(sql, "\"");

		return AG_SUCCESS;
	}

	int FeatureType::BuildQuerySQL(char* sql, uint size, Envelope& extent, GeometryFormat format/*=WKB*/)
	{	
		bool b = true;
		Field *f = NULL;
		const char* fname = NULL;		
		std::vector<Field*>::iterator iter;

		std::vector<Field*>& fields = GetFields();
		ag_snprintf(sql, size, "select ");
		for(iter=fields.begin(); iter!=fields.end(); iter++)
		{
			if(b)
				b = false;
			else
				 strcat(sql, ",");

			f = *iter;
			fname = f->GetName();
			if(f->GetType()==augeFieldTypeGeometry)
			{
				switch(format)
				{
				case WKT:
					strcat(sql, "st_astext(");
					break;
				case WKB:
					strcat(sql, "st_asbinary(");
					break;
				case GML2:
					strcat(sql, "st_asgml(");
					break;
					//case GML3:
					//	break;
				}
				strcat(sql, "\"");
				strcat(sql, fname);
				strcat(sql, "\"");
				strcat(sql, ") as ");				
				strcat(sql, "\"");
				strcat(sql, fname);
				strcat(sql, "\"");
			}
			else
			{
				strcat(sql, "\"");
				strcat(sql, fname);
				strcat(sql, "\"");
			}
		}
		strcat(sql, " from ");
		strcat(sql, "\"");
		strcat(sql, m_name.c_str());
		strcat(sql, "\"");

		return AG_SUCCESS;
	}
}