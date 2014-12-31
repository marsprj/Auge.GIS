#include "GetFeatureRequest.h"
#include "AugeFilter.h"

namespace auge
{
	GetFeatureRequest::GetFeatureRequest()
	{
		m_version = "1.1.0";
		m_mime_type = "text/xml";
		m_max_features = -1;
		m_pFilter = NULL;
		m_orderby_type = augeOrderAsc;
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

	void GetFeatureRequest::SetFilter(const char* xml)
	{
		if(xml==NULL)
		{
			return;
		}
		FilterReader* reader = NULL;
		FilterFactory* pFactory = augeGetFilterFactoryInstance();
		reader = pFactory->CreateFilerReader(NULL);

		if(m_pFilter!=NULL)
		{
			m_pFilter->Release();
			m_pFilter = NULL;
		}
		m_pFilter = reader->Read(xml);
		reader->Release();		
	}

	void GetFeatureRequest::SetFilter(XElement* pxFilter)
	{
		if(pxFilter==NULL)
		{
			return;
		}
		FilterReader* reader = NULL;
		FilterFactory* pFactory = augeGetFilterFactoryInstance();
		reader = pFactory->CreateFilerReader(NULL);

		if(m_pFilter!=NULL)
		{
			m_pFilter->Release();
			m_pFilter = NULL;
		}
		m_pFilter = reader->Read(pxFilter);
		reader->Release();		
	}

	GFilter* GetFeatureRequest::GetFilter()
	{
		return m_pFilter;
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

	void GetFeatureRequest::SetMaxFeatures(const char* maxFeatures)
	{
		if(maxFeatures==NULL)
		{
			m_max_features = -1;
		}
		if(strlen(maxFeatures)==0)
		{
			m_max_features = -1;
		}
		
		m_max_features = atoi(maxFeatures);
	}

	bool GetFeatureRequest::Create(GPropertySet& props)
	{
		const char* val = NULL;
		val = props.GetValue("version");
		SetVersion(val);
		val = props.GetValue("typeName");
		SetTypeName(val);
		val = props.GetValue("filter");
		SetFilter(val);
		val = props.GetValue("maxFeatures");
		SetMaxFeatures(val);
		return true;
	}

	bool GetFeatureRequest::Create(XElement* pxRequest)
	{
		GLogger* pLogger = augeGetLoggerInstance();
		pLogger->Info("GetFeature Post", __FILE__, __LINE__);

		const char* val = NULL;
		XAttribute* pxAttr = NULL;
		pxAttr = pxRequest->GetAttribute("version");
		if(pxAttr==NULL)
		{
			return false;
		}
		val = pxAttr->GetValue();
		SetVersion(val);

		pxAttr = pxRequest->GetAttribute("maxFeatures");
		if(pxAttr==NULL)
		{
			return false;
		}
		val = pxAttr->GetValue();
		SetMaxFeatures(val);
		
		XElement* pxQuery = (XElement*)pxRequest->GetFirstChild("Query");
		if(pxQuery==NULL)
		{
			return false;
		}
		// typeName
		pxAttr = pxQuery->GetAttribute("typeName");
		if(pxAttr==NULL)
		{
			return false;
		}
		val = pxAttr->GetValue();
		SetTypeName(val);

		// filter
		XElement* pxFilter = (XElement*)pxQuery->GetFirstChild("Filter");
		if(pxFilter!=NULL)
		{
			SetFilter(pxFilter);
		}

		// orderBy
		XElement* pxOrderBy = (XElement*)pxQuery->GetFirstChild("OrderBy");
		if(pxOrderBy!=NULL)
		{
			SetOrderBy(pxOrderBy);
		}

		return true;
	}

	void GetFeatureRequest::SetOrderBy(XElement* pxOrderBy)
	{
		XElement* pxProperty = (XElement*)pxOrderBy->GetFirstChild("Property");
		if(pxProperty==NULL)
		{
			return;
		}
		XAttribute* pxAttr = NULL;
		pxAttr = pxProperty->GetAttribute("name");
		if(pxAttr!=NULL)
		{
			const char* str = pxAttr->GetValue();
			if(str!=NULL)
			{
				m_orderby_field = str;
			}
			else
			{
				m_orderby_field.clear();
			}
		}
		pxAttr = pxProperty->GetAttribute("name");
		if(pxAttr!=NULL)
		{
			const char* str = pxAttr->GetValue();
			if(str!=NULL)
			{
				if(!g_stricmp(str, "Asc"))
				{
					m_orderby_type = augeOrderAsc;
				}
				else if(!g_stricmp(str, "Desc"))
				{
					m_orderby_type = augeOrderDesc;
				}
			}
		}
	}
}