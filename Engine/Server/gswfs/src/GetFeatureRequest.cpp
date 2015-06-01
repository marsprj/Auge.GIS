#include "GetFeatureRequest.h"
#include "AugeField.h"
#include "AugeData.h"
#include "AugeCarto.h"
#include "AugeXML.h"
#include "AugeFilter.h"
#include "AugeWebCore.h"

namespace auge
{	
	GetFeatureRequest::GetFeatureRequest()
	{
		m_version = "1.1.0";
		m_mime_type = "text/xml";
		m_output_format = AUGE_WFS_OUTPUT_FORMAT_GML2;
		m_max_features = 10;
		m_offset = 0;
		m_encoding = "GBK";

		m_fields = NULL;
		m_filter = NULL;

		//m_pFilter = NULL;
		m_pQuery = NULL;

		m_pxDoc = NULL;
	}

	GetFeatureRequest::~GetFeatureRequest()
	{
		//AUGE_SAFE_RELEASE(m_pFilter);
		AUGE_SAFE_RELEASE(m_pQuery);
	}

	//const char*	GetFeatureRequest::GetEngine()
	//{
	//	return "wfs";
	//}

	//const char*	GetFeatureRequest::GetVersion()
	//{
	//	return m_version.c_str();
	//}

	const char* GetFeatureRequest::GetTypeName()
	{
		return m_type_name.c_str();
	}

	const char*	GetFeatureRequest::GetRequest()
	{
		return "GetFeature";
	}

	//void GetFeatureRequest::SetVersion(const char* value)
	//{
	//	if(value==NULL)
	//	{
	//		return;
	//	}
	//	m_version = value;
	//}

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

	void GetFeatureRequest::SetEncoding(const char* encoding)
	{
		if((encoding==NULL)||(strlen(encoding)==0))
		{
			m_encoding = "GBK";
		}
		else
		{
			m_encoding = encoding;
		}
	}

	const char* GetFeatureRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	//const char*	GetFeatureRequest::GetMimeType()
	//{
	//	return m_mime_type.c_str();
	//}

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
			m_output_format = AUGE_WFS_OUTPUT_FORMAT_GML2;
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

	GQuery* GetFeatureRequest::GetQuery(FeatureClass* pFeatureClass)
	{
		if(m_pQuery==NULL)
		{
			switch(GetMethod())
			{
			case augeHttpGet:
				m_pQuery = ParseKvpQuery(pFeatureClass);
				break;
			case augeHttpPost:
				m_pQuery = ParseXmlQuery(pFeatureClass);
				break;
			}
			
		}
		return m_pQuery;
	}

	GQuery* GetFeatureRequest::ParseXmlQuery(FeatureClass* pFeatureClass)
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

		FilterFactory* pFilterFactory = augeGetFilterFactoryInstance();
		GQuery* pQuery = pFilterFactory->CreateQuery();
		if(pQuery==NULL)
		{
			return NULL;
		}

		XElement* pxFilter = (XElement*)pxQuery->GetFirstChild("Filter");
		if(pxFilter!=NULL)
		{
			GFilter* pFilter = NULL;
			FilterReader* reader = pFilterFactory->CreateFilerReader(pFeatureClass->GetFields());
			pFilter = reader->Read(pxFilter);
			pQuery->SetFilter(pFilter);

		}

		//PropertyName
		char field_name[AUGE_NAME_MAX];
		const char* property_name;
		XNode* pxNode = NULL;
		XNodeSet* pxNodeSet = pxQuery->GetChildren("PropertyName");
		pxNodeSet->Reset();
		while((pxNode=pxNodeSet->Next())!=NULL)
		{
			property_name = pxNode->GetContent();
			ParseFieldName(property_name, field_name, AUGE_NAME_MAX);
			pQuery->AddSubField(field_name);

		}

		if(m_max_features>0)
		{
			pQuery->SetMaxFeatures(m_max_features);
		}
		if(m_offset>0)
		{
			pQuery->SetOffset(m_offset);
		}
		
		pxNodeSet->Release();

		return pQuery;
	}

	GQuery* GetFeatureRequest::ParseKvpQuery(FeatureClass* pFeatureClass)//;const char* filter, const char* fields, const char* typeName, Map* pMap)
	{
		XParser parser;
		XDocument* pxDoc = NULL;

		GFilter	*pFilter = NULL;
		FilterReader* reader = NULL;
		FilterFactory *factory = augeGetFilterFactoryInstance();
		GQuery* pQuery = factory->CreateQuery();
		if(pQuery==NULL)
		{
			return NULL;
		}

		pQuery = factory->CreateQuery();
		if(m_extent.IsValid())
		{
			GField* pField = pFeatureClass->GetFields()->GetGeometryField();
			if(pField)
			{
				pFilter = factory->CreateBBoxFilter(pField->GetName(), m_extent);
				pQuery->SetFilter(pFilter);
			}
		}
		else
		{
			pxDoc = parser.ParseMemory(m_filter);
			if(pxDoc!=NULL)
			{
				reader = factory->CreateFilerReader(pFeatureClass->GetFields());
				pFilter = reader->Read(pxDoc->GetRootNode());
				pQuery->SetFilter(pFilter);
				pxDoc->Release();
			}
		}

		pQuery->SetMaxFeatures(m_max_features);
		pQuery->SetOffset(m_offset);

		SetFields(pQuery, m_fields);

		return pQuery;
	}

	void GetFeatureRequest::SetFields(GQuery* pQuery, const char* fields)
	{
		if(fields==NULL)
		{
			return;
		}

		char  field[AUGE_NAME_MAX];
		char* field_c = strdup(fields);
		char* ptr = strtok(field_c, ",");
		while(ptr!=NULL)
		{
			ParseFieldName(ptr, field, AUGE_NAME_MAX);
			pQuery->AddSubField(field);

			ptr = strtok(NULL, ",");
		}

		free(field_c);
	}

	bool GetFeatureRequest::Create(rude::CGI& cgi, Map* pMap)
	{
		SetVersion(cgi["version"]);
		SetTypeName(cgi["typeName"]);
		
		SetSourceName(cgi["sourceName"]);
		SetMapName(cgi["mapName"]);

		SetOutputFormat(cgi["outputFormat"]);
		SetMaxFeatures(cgi["maxFeatures"]);
		SetOffset(cgi["offset"]);
		SetBBox(cgi["bbox"]);

		SetEncoding(cgi["encoding"]);

		m_filter = cgi["filter"];
		m_fields = cgi["fields"];
		//if(!m_extent.IsValid())
		//{
		//	SetQuery(cgi["filter"],cgi["fields"], GetTypeName(), pMap);
		//}

		return true;
	}

	bool GetFeatureRequest::Create(XDocument* pxDoc)
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

		pxAttr = pxRoot->GetAttribute("encoding");
		if(pxAttr!=NULL)
		{
			SetEncoding(pxAttr->GetValue());
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

		//Layer* pLayer = NULL;
		//pLayer = pMap->GetLayer(m_type_name.c_str());
		//if(pLayer==NULL)
		//{
		//	return false;
		//}
		//if(pLayer->GetType()!=augeLayerFeature)
		//{
		//	return false;
		//}
		//FeatureLayer* pFLayer = NULL;
		//FeatureClass* pFeatureClass = NULL;
		//pFLayer = static_cast<FeatureLayer*>(pLayer);
		//pFeatureClass = pFLayer->GetFeatureClass();
		//if(pFeatureClass==NULL)
		//{
		//	return false;
		//}

		//FilterFactory* pFilterFactory = augeGetFilterFactoryInstance();
		//m_pQuery = pFilterFactory->CreateQuery();

		//XElement* pxFilter = (XElement*)pxQuery->GetFirstChild("Filter");
		//if(pxFilter!=NULL)
		//{
		//	GFilter* pFilter = NULL;
		//	FilterReader* reader = pFilterFactory->CreateFilerReader(pFeatureClass->GetFields());
		//	pFilter = reader->Read(pxFilter);
		//	m_pQuery->SetFilter(pFilter);

		//}

		////PropertyName
		//char field_name[AUGE_NAME_MAX];
		//const char* property_name;
		//XNode* pxNode = NULL;
		//XNodeSet* pxNodeSet = pxQuery->GetChildren("PropertyName");
		//pxNodeSet->Reset();
		//while((pxNode=pxNodeSet->Next())!=NULL)
		//{
		//	property_name = pxNode->GetContent();
		//	ParseFieldName(property_name, field_name, AUGE_NAME_MAX);
		//	m_pQuery->AddSubField(field_name);

		//}
		//pxNodeSet->Release();

		return true;
	}
	
	//bool GetFeatureRequest::Create(XDocument* pxDoc,Map* pMap)
	//{
	//	XElement	*pxRoot = NULL;
	//	XAttribute	*pxAttr = NULL;

	//	pxRoot = pxDoc->GetRootNode();
	//	pxAttr = pxRoot->GetAttribute("version");
	//	if(pxAttr!=NULL)
	//	{
	//		SetVersion(pxAttr->GetValue());
	//	}

	//	pxAttr = pxRoot->GetAttribute("mapName");
	//	if(pxAttr!=NULL)
	//	{
	//		SetMapName(pxAttr->GetValue());
	//	}

	//	pxAttr = pxRoot->GetAttribute("outputFormat");
	//	if(pxAttr!=NULL)
	//	{
	//		SetOutputFormat(pxAttr->GetValue());
	//	}

	//	pxAttr = pxRoot->GetAttribute("offset");
	//	if(pxAttr!=NULL)
	//	{
	//		SetOffset(pxAttr->GetValue());
	//	}

	//	XElement* pxQuery = (XElement*)pxRoot->GetFirstChild("Query");
	//	if(pxQuery==NULL)
	//	{
	//		return false;
	//	}

	//	if(m_pQuery!=NULL)
	//	{
	//		AUGE_SAFE_RELEASE(m_pQuery);
	//	}
	//	
	//	pxAttr = pxQuery->GetAttribute("typeName");
	//	if(pxAttr==NULL)
	//	{
	//		return false;
	//	}
	//	SetTypeName(pxAttr->GetValue());
	//	if(m_type_name.empty())
	//	{
	//		return false;
	//	}
	//	
	//	Layer* pLayer = NULL;
	//	pLayer = pMap->GetLayer(m_type_name.c_str());
	//	if(pLayer==NULL)
	//	{
	//		return false;
	//	}
	//	if(pLayer->GetType()!=augeLayerFeature)
	//	{
	//		return false;
	//	}
	//	FeatureLayer* pFLayer = NULL;
	//	FeatureClass* pFeatureClass = NULL;
	//	pFLayer = static_cast<FeatureLayer*>(pLayer);
	//	pFeatureClass = pFLayer->GetFeatureClass();
	//	if(pFeatureClass==NULL)
	//	{
	//		return false;
	//	}

	//	FilterFactory* pFilterFactory = augeGetFilterFactoryInstance();
	//	m_pQuery = pFilterFactory->CreateQuery();

	//	XElement* pxFilter = (XElement*)pxQuery->GetFirstChild("Filter");
	//	if(pxFilter!=NULL)
	//	{
	//		GFilter* pFilter = NULL;
	//		FilterReader* reader = pFilterFactory->CreateFilerReader(pFeatureClass->GetFields());
	//		pFilter = reader->Read(pxFilter);
	//		m_pQuery->SetFilter(pFilter);
	//		
	//	}

	//	//PropertyName
	//	char field_name[AUGE_NAME_MAX];
	//	const char* property_name;
	//	XNode* pxNode = NULL;
	//	XNodeSet* pxNodeSet = pxQuery->GetChildren("PropertyName");
	//	pxNodeSet->Reset();
	//	while((pxNode=pxNodeSet->Next())!=NULL)
	//	{
	//		property_name = pxNode->GetContent();
	//		ParseFieldName(property_name, field_name, AUGE_NAME_MAX);
	//		m_pQuery->AddSubField(field_name);
	//		
	//	}
	//	pxNodeSet->Release();

	//	return true;
	//}

	// world:name --> name
	void GetFeatureRequest::ParseFieldName(const char* property_name, char* field_name, size_t size)
	{
		const char* ptr = strstr(property_name,":");
		if(ptr)
		{
			g_snprintf(field_name,size,"%s", ptr+1);
		}
		else
		{
			g_snprintf(field_name,size,"%s", property_name);
		}
	}

	//const char*	GetFeatureRequest::GetServiceName()
	//{
	//	return m_service_name.c_str();
	//}

	//const char* GetFeatureRequest::GetServiceURI()
	//{
	//	return m_service_uri.c_str();
	//}

	//void GetFeatureRequest::SetServiceName(const char* name)
	//{
	//	if(name==NULL)
	//	{
	//		m_service_name.clear();
	//	}
	//	else
	//	{
	//		m_service_name = name;
	//	}
	//}
	//void GetFeatureRequest::SetServiceURI(const char* uri)
	//{
	//	if(uri==NULL)
	//	{
	//		m_service_uri.clear();
	//	}
	//	else
	//	{
	//		m_service_uri = uri;
	//	}
	//}

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

	augeHttpMethodType GetFeatureRequest::GetMethod()
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