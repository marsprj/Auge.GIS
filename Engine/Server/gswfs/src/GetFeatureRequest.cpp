#include "GetFeatureRequest.h"
#include "AugeData.h"
#include "AugeCarto.h"
#include "AugeXML.h"
#include "AugeFilter.h"

namespace auge
{
	GetFeatureRequest::GetFeatureRequest()
	{
		m_version = "1.1.0";
		m_mime_type = "text/xml";
		m_output_format = "GML2";
		m_max_features = 10;
		m_offset = 0;

		m_pFilter = NULL;
	}

	GetFeatureRequest::~GetFeatureRequest()
	{
		if(m_pFilter!=NULL)
		{
			m_pFilter->Release();
			m_pFilter = NULL;
		}
	}

	const char*	GetFeatureRequest::GetEngine()
	{
		return "wfs";
	}

	const char*	GetFeatureRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char* GetFeatureRequest::GetTypeName()
	{
		return m_type_name.c_str();
	}

	const char*	GetFeatureRequest::GetRequest()
	{
		return "GetFeature";
	}

	void GetFeatureRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	void GetFeatureRequest::SetTypeName(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		const char* sep = strstr(value,":");
		m_full_name = value;
		m_type_name = sep==NULL ? value : sep+1;
	}

	const char*	GetFeatureRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	const char*	GetFeatureRequest::GetOutputFormat()
	{
		return m_output_format.c_str();
	}

	g_int GetFeatureRequest::GetMaxFeatures()
	{
		return m_max_features;
	}

	g_int GetFeatureRequest::GetOffset()
	{
		return m_offset;
	}

	void GetFeatureRequest::SetOutputFormat(const char* format)
	{
		if(format==NULL)
		{
			m_output_format = "GML2";
		}
		else if(format==NULL)
		{
			m_output_format = "GML2";
		}
		else
		{
			m_output_format = format;
		}
	}

	void GetFeatureRequest::SetMaxFeatures(const char* maxFeatures)
	{
		if(maxFeatures==NULL)
		{
			m_max_features = 10;
		}
		//else if(!isdigit(maxFeatures))
		//{
		//	m_max_features = 10;
		//}
		else
		{
			m_max_features = atoi(maxFeatures);
		}
	}

	void GetFeatureRequest::SetOffset(const char* offset)
	{
		if(offset==NULL)
		{
			m_offset = 0;
		}
		//else if(!isdigit(maxFeatures))
		//{
		//	m_max_features = 10;
		//}
		else
		{
			m_offset = atoi(offset);
		}
	}

	GEnvelope& GetFeatureRequest::GetBBox()
	{
		return m_extent;
	}

	void GetFeatureRequest::SetBBox(const char* bbox)
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

	GFilter* GetFeatureRequest::GetFilter()
	{
		return m_pFilter;
	}

	void GetFeatureRequest::SetFilter(const char* filter, const char* typeName, Map* pMap)
	{
		XParser parser;
		XDocument* pxDoc = NULL;
		if(typeName==NULL||pMap==NULL)
		{
			return;
		}

		GFilter* pFilter = NULL;
		FilterReader* reader = NULL;
		FilterFactory *factory = augeGetFilterFactoryInstance();

		Layer* pLayer = pMap->GetLayer(typeName);
		if(pLayer==NULL)
		{
			return;
		}
		//augeLayerType ltype = pLayer->GetType();
		//if(ltype!=augeLayerFeature);
		//{
		//	return;
		//}

		FeatureLayer* pFeatureLayer = static_cast<FeatureLayer*>(pLayer);
		FeatureClass* pFeatureClass = pFeatureLayer->GetFeatureClass();
		if(pFeatureClass==NULL)
		{
			return;
		}

		pxDoc = parser.ParseMemory(filter);
		if(pxDoc==NULL)
		{
			return;
		}

		reader = factory->CreateFilerReader(pFeatureClass->GetFields());
		pFilter = reader->Read(pxDoc->GetRootNode());
		pxDoc->Close();
		pxDoc->Release();

		if(m_pFilter!=NULL)
		{
			m_pFilter->Release();
			m_pFilter = NULL;
		}
		m_pFilter = pFilter;
	}

	bool GetFeatureRequest::Create(rude::CGI& cgi, Map* pMap)
	{
		SetVersion(cgi["version"]);
		SetTypeName(cgi["typeName"]);
		SetOutputFormat(cgi["outputFormat"]);
		SetMaxFeatures(cgi["maxFeatures"]);
		SetOffset(cgi["offset"]);
		SetBBox(cgi["bbox"]);
		SetFilter(cgi["filter"], GetTypeName(), pMap);
		return true;
	}
	
	bool GetFeatureRequest::Create(XDocument* pxDoc,Map* pMap)
	{
		XElement	*pxRoot = NULL;
		XAttribute	*pxAttr = NULL;

		pxRoot = pxDoc->GetRootNode();
		pxAttr = pxRoot->GetAttribute("version");
		if(pxAttr!=NULL)
		{
			SetVersion(pxAttr->GetValue());
		}

		pxAttr = pxRoot->GetAttribute("outputFormat");
		if(pxAttr!=NULL)
		{
			SetOutputFormat(pxAttr->GetValue());
		}

		pxAttr = pxRoot->GetAttribute("offset");
		if(pxAttr!=NULL)
		{
			SetOffset(pxAttr->GetValue());
		}

		XElement* pxQuery = (XElement*)pxRoot->GetFirstChild("Query");
		if(pxQuery==NULL)
		{
			return false;
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

		XElement* pxFilter = (XElement*)pxQuery->GetFirstChild("Filter");
		if(pxFilter!=NULL)
		{
			if(m_pFilter!=NULL)
			{
				AUGE_SAFE_RELEASE(m_pFilter);
				//m_pFilter = decode_filter(pxFilter);
			}
		}
		return true;
	}

	const char*	GetFeatureRequest::GetServiceName()
	{
		return m_service_name.c_str();
	}

	const char* GetFeatureRequest::GetServiceURI()
	{
		return m_service_uri.c_str();
	}

	void GetFeatureRequest::SetServiceName(const char* name)
	{
		if(name==NULL)
		{
			m_service_name.clear();
		}
		else
		{
			m_service_name = name;
		}
	}
	void GetFeatureRequest::SetServiceURI(const char* uri)
	{
		if(uri==NULL)
		{
			m_service_uri.clear();
		}
		else
		{
			m_service_uri = uri;
		}
	}

	void GetFeatureRequest::Info()
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

		value = GetOutputFormat();
		g_sprintf(str,"\t%s:%s", "OutputFormat", (value==NULL)?"":value);
		pLogger->Debug(str);

		g_sprintf(str,"\t%s:%d", "MaxFeatures", GetMaxFeatures());
		pLogger->Debug(str);

		g_sprintf(str,"\t%s:%d", "Offset", GetOffset());
		pLogger->Debug(str);
	}

}