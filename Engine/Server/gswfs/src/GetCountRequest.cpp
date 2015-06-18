#include "GetCountRequest.h"
#include "AugeData.h"
#include "AugeCarto.h"
#include "AugeXML.h"
#include "AugeFilter.h"
#include "AugeWebCore.h"

namespace auge
{	
	GetCountRequest::GetCountRequest()
	{
		m_version = "1.1.0";
		m_mime_type = "text/xml";
		m_filter = NULL;

		m_pFilter = NULL;
		m_pxDoc = NULL;
	}

	GetCountRequest::~GetCountRequest()
	{
		AUGE_SAFE_RELEASE(m_pFilter);
	}

	const char* GetCountRequest::GetTypeName()
	{
		return m_type_name.c_str();
	}

	const char*	GetCountRequest::GetRequest()
	{
		return "GetCount";
	}

	void GetCountRequest::SetTypeName(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		const char* sep = strstr(value,":");
		m_full_name = value;
		m_type_name = sep==NULL ? value : sep+1;
	}
	
	GEnvelope& GetCountRequest::GetBBox()
	{
		return m_extent;
	}

	void GetCountRequest::SetBBox(const char* bbox)
	{
		if(bbox==NULL)
		{
			m_bbox.clear();
		}
		else if(!strlen(bbox))
		{
			m_bbox.clear();
		}
		else
		{
			double xmin, ymin, xmax, ymax;
			sscanf(bbox, "%lf,%lf,%lf,%lf,",&xmin, &ymin, &xmax, &ymax);
			m_extent.Set(xmin, ymin, xmax, ymax);
		}
	}

	GFilter* GetCountRequest::GetFilter(FeatureClass* pFeatureClass)
	{
		if(m_pFilter==NULL)
		{
			switch(GetMethod())
			{
			case augeHttpGet:
				m_pFilter = ParseKvpFilter(pFeatureClass);
				break;
			case augeHttpPost:
				m_pFilter = ParseXmlFilter(pFeatureClass);
				break;
			}
			
		}
		return m_pFilter;
	}

	GFilter* GetCountRequest::ParseXmlFilter(FeatureClass* pFeatureClass)
	{
		if(m_pxDoc==NULL)
		{
			return NULL;
		}

		XElement* pxRoot = m_pxDoc->GetRootNode();
		XElement* pxQuery = (XElement*)pxRoot->GetFirstChild("Query");
		if(pxQuery==NULL)
		{
			return NULL;
		}

		GFilter* pFilter = NULL;
		FilterFactory* pFilterFactory = augeGetFilterFactoryInstance();
		XElement* pxFilter = (XElement*)pxQuery->GetFirstChild("Filter");
		if(pxFilter!=NULL)
		{	
			FilterReader* reader = pFilterFactory->CreateFilerReader(pFeatureClass->GetFields());
			pFilter = reader->Read(pxFilter);
			reader->Release();
		}
		return pFilter;
	}

	GFilter* GetCountRequest::ParseKvpFilter(FeatureClass* pFeatureClass)//;const char* filter, const char* fields, const char* typeName, Map* pMap)
	{
		XParser parser;
		XDocument* pxDoc = NULL;
		//if(typeName==NULL||pMap==NULL)
		//{
		//	return;
		//}

		GFilter	*pFilter = NULL;
		FilterReader* reader = NULL;
		FilterFactory *factory = augeGetFilterFactoryInstance();

		pxDoc = parser.ParseMemory(m_filter);
		if(pxDoc!=NULL)
		{
			reader = factory->CreateFilerReader(pFeatureClass->GetFields());
			pFilter = reader->Read(pxDoc->GetRootNode());
			pxDoc->Release();
		}

		return pFilter;
	}

	bool GetCountRequest::Create(rude::CGI& cgi, Map* pMap)
	{
		SetVersion(cgi["version"]);
		SetTypeName(cgi["typeName"]);
		
		SetSourceName(cgi["sourceName"]);
		SetMapName(cgi["mapName"]);

		SetBBox(cgi["bbox"]);
		m_filter = cgi["filter"];
		
		return true;
	}

	bool GetCountRequest::Create(XDocument* pxDoc)
	{
		XElement	*pxRoot = NULL;
		XAttribute	*pxAttr = NULL;

		m_pxDoc = pxDoc;

		pxRoot = pxDoc->GetRootNode();
		pxAttr = pxRoot->GetAttribute("version");
		if(pxAttr!=NULL)
		{
			SetVersion(pxAttr->GetValue());
		}

		pxAttr = pxRoot->GetAttribute("mapName");
		if(pxAttr!=NULL)
		{
			SetMapName(pxAttr->GetValue());
		}

		pxAttr = pxRoot->GetAttribute("sourceName");
		if(pxAttr!=NULL)
		{
			SetSourceName(pxAttr->GetValue());
		}
		
		XElement* pxQuery = (XElement*)pxRoot->GetFirstChild("Query");
		if(pxQuery==NULL)
		{
			return NULL;
		}

		pxAttr = pxQuery->GetAttribute("typeName");
		if(pxAttr==NULL)
		{
			return false;
		}
		SetTypeName(pxAttr->GetValue());
		if(m_type_name.empty())
		{
			return false;
		}

		return true;
	}
	

	void GetCountRequest::Info()
	{
		GLogger* pLogger = augeGetLoggerInstance();
		char str[AUGE_MSG_MAX];
		const char* value = GetRequest();
		pLogger->Debug("[Requet Parameters]");
		g_sprintf(str,"\t%s:%s", "Request", (value==NULL)?"":value);
		pLogger->Debug(str);
		
		value = GetVersion();
		g_sprintf(str,"\t%s:%s", "Version", (value==NULL)?"":value);
		pLogger->Debug(str);

		g_sprintf(str,"\t%s:%s", "Service", "wfs");
		pLogger->Debug(str);

		value = GetMimeType();
		g_sprintf(str,"\t%s:%s", "MimeType", (value==NULL)?"":value);
		pLogger->Debug(str);

		value = GetTypeName();
		g_sprintf(str,"\t%s:%s", "TypeName", (value==NULL)?"":value);
		pLogger->Debug(str);

	}

	augeHttpMethodType GetCountRequest::GetMethod()
	{
		const char* method = getenv("REQUEST_METHOD");
		if(method == NULL)
		{
			return augeHttpGet;
		}

		if(strcmp(method, "GET")==0)
		{
			return augeHttpGet;
		}
		else if(strcmp(method, "POST")==0)
		{
			return augeHttpPost;
		}
		return augeHttpGet;
	}

}