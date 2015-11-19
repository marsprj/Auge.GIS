#include "FeatureImportRequest.h"
#include "AugeWebCore.h"

namespace auge
{
	FeatureImportRequest::FeatureImportRequest()
	{
		m_srid = 4326;
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	FeatureImportRequest::~FeatureImportRequest()
	{

	}

	const char*	FeatureImportRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	const char*	FeatureImportRequest::GetUser()
	{
		return m_user.empty() ? NULL : m_user.c_str();
	}

	const char*	FeatureImportRequest::GetEngine()
	{
		return "gps";
	}

	const char*	FeatureImportRequest::GetVersion()
	{
		return m_user.empty() ? NULL : m_user.c_str();
	}

	const char*	FeatureImportRequest::GetRequest()
	{
		return "FeatureImport";
	}

	const char*	FeatureImportRequest::GetSourceName()
	{
		return m_source_name.empty() ? NULL : m_source_name.c_str();
	}

	const char*	FeatureImportRequest::GetShapePath()
	{
		return m_shp_path.empty() ? NULL : m_shp_path.c_str();
	}

	const char*	FeatureImportRequest::GetShapeName()
	{
		return m_shp_name.empty() ? NULL : m_shp_name.c_str();
	}

	const char* FeatureImportRequest::GetTypeName()
	{
		return m_type_name.empty() ? NULL : m_type_name.c_str();
	}

	g_uint FeatureImportRequest::GetSrid()
	{
		return m_srid;
	}

	void FeatureImportRequest::SetSourceName(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_source_name.clear();
		}
		else
		{
			m_source_name = sourceName;
		}
	}

	void FeatureImportRequest::SetTypeName(const char* typeName)
	{
		if(typeName==NULL)
		{
			m_type_name.clear();
		}
		else
		{
			WebContext* pWebContext = augeGetWebContextInstance();			
			m_type_name = pWebContext->ParameterEncoding(typeName);
			//m_type_name = typeName;
		}
	}

	void FeatureImportRequest::SetShapePath(const char* path)
	{
		if(path==NULL)
		{
			m_shp_path.clear();
		}
		else
		{
			WebContext* pWebContext = augeGetWebContextInstance();			
			m_shp_path = pWebContext->ParameterEncoding(path);
			//m_shp_path = path;
		}
	}

	void FeatureImportRequest::SetShapeName(const char* name)
	{
		if(name==NULL)
		{
			m_shp_name.clear();
		}
		else
		{
			WebContext* pWebContext = augeGetWebContextInstance();		
			m_shp_name = pWebContext->ParameterEncoding(name);
			//m_shp_name = name;
		}
	}

	void FeatureImportRequest::SetSrid(const char* srid)
	{
		if(srid==NULL)
		{
			m_srid = 4326;
		}
		else
		{
			if(isdigit(srid[0]))
			{
				m_srid = atoi(srid);
			}
			else
			{
				m_srid = 4326;
			}
		}
	}

	bool FeatureImportRequest::Create(rude::CGI& cgi)
	{
		SetSourceName(cgi["sourceName"]);
		SetTypeName(cgi["typeName"]);
		SetShapePath(cgi["shppath"]);
		SetShapeName(cgi["shpname"]);
		SetSrid(cgi["srid"]);
		return true;
	}
}