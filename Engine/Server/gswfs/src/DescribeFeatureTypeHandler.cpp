#include "DescribeFeatureTypeHandler.h"
#include "DescribeFeatureTypeRequest.h"
#include "DescribeFeatureTypeResponse.h"
#include "WFeatureEngine.h"
#include "AugeService.h"
#include "AugeXML.h"
#include "AugeCarto.h"
#include "AugeFeature.h"
#include "AugeField.h"

namespace auge
{
	DescribeFeatureTypeHandler::DescribeFeatureTypeHandler()
	{ 

	}

	DescribeFeatureTypeHandler::~DescribeFeatureTypeHandler()
	{

	}

	const char*	DescribeFeatureTypeHandler::GetName()
	{
		return "DescribeFeatureType";
	}

	WebRequest*	DescribeFeatureTypeHandler::ParseRequest(rude::CGI& cgi)
	{
		DescribeFeatureTypeRequest* pRequest = new DescribeFeatureTypeRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			return NULL;
		}
		return pRequest;
	}

	WebRequest*	DescribeFeatureTypeHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		DescribeFeatureTypeRequest* pRequest = new DescribeFeatureTypeRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			return NULL;
		}
		pRequest->SetMapName(mapName);
		return pRequest;
	}

	//WebRequest*	DescribeFeatureTypeHandler::ParseRequest(rude::CGI& cgi, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	//{
	//	DescribeFeatureTypeRequest* pRequest = new DescribeFeatureTypeRequest();
	//	if(!pRequest->Create(cgi))
	//	{
	//		GLogger* pLogger = augeGetLoggerInstance();
	//		pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
	//		pRequest->Release();
	//		pRequest = NULL;
	//	}
	//	return pRequest;
	//}

	WebRequest*	DescribeFeatureTypeHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		DescribeFeatureTypeRequest* pRequest = new DescribeFeatureTypeRequest();
		//if(!pRequest->Create(cgi))
		//{
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//	pRequest->Release();
		//	pRequest = NULL;
		//}
		return pRequest;
	}

	WebResponse* DescribeFeatureTypeHandler::Execute(WebRequest* pWebRequest)
	{
		WebResponse* pWebResponse = NULL;
		DescribeFeatureTypeRequest* pRequest = static_cast<DescribeFeatureTypeRequest*>(pWebRequest);

		const char* version = pRequest->GetVersion();
		if(strcmp(version,"1.0.0")==0)
		{

		}
		else if(strcmp(version,"1.1.0")==0)
		{

		}
		else
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "WMS does not support %s",version);
			pExpResponse->SetMessage(msg); 
			pWebResponse = pExpResponse;
		}

		return pWebResponse;
	}

	WebResponse* DescribeFeatureTypeHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		WebResponse* pWebResponse = NULL;
		DescribeFeatureTypeRequest* pRequest = static_cast<DescribeFeatureTypeRequest*>(pWebRequest);
		
		//if(pRequest->IsValidSource())
		//{
		//	pWebResponse = ExecuteBySource(pWebRequest, pWebContext);
		//}
		//else
		//{
		//	pWebResponse = ExecuteByMap(pWebRequest, pWebContext);
		//}

		FeatureClass* pFeatureClass = GetFeatureClass(pWebRequest, pWebContext);
		if(pFeatureClass==NULL)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		char cache_name[AUGE_NAME_MAX];
		char cache_file[AUGE_PATH_MAX];
		const char* typeName = pRequest->GetTypeName();
		const char* cache_path = pWebContext->GetCacheProtocolPath();
		g_snprintf(cache_name, AUGE_NAME_MAX,"%s_wfs_%s_describe_feauretype_1_1_0", pWebContext->GetService(), typeName);
		auge_make_path(cache_file, NULL, cache_path, cache_name,"xml");

		//if(access(capa_path,4))
		{
			WriteDescribeFeatureType(pRequest->GetVersion(), pWebContext, typeName, pFeatureClass);
		}

		pFeatureClass->Release();

		DescribeFeatureTypeResponse* pResponse = new DescribeFeatureTypeResponse(pRequest);		
		pResponse->SetPath(cache_file);

		return pResponse;
	}

	FeatureClass* DescribeFeatureTypeHandler::GetFeatureClass(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		FeatureClass* pFeatureClass = NULL;
		WFeatureRequest* pRequest = static_cast<WFeatureRequest*>(pWebRequest);
		if(pRequest->IsValidSource())
		{
			pFeatureClass = GetFeatureClassBySource(pWebRequest, pWebContext);
		}
		else
		{
			pFeatureClass = GetFeatureClassByMap(pWebRequest, pWebContext);
		}
		return pFeatureClass;
	}

	FeatureClass* DescribeFeatureTypeHandler::GetFeatureClassByMap(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		const char* typeName = NULL;
		Layer* pLayer = NULL;
		GLogger *pLogger = augeGetLoggerInstance();
		DescribeFeatureTypeRequest* pRequest = static_cast<DescribeFeatureTypeRequest*>(pWebRequest);
		typeName = pRequest->GetTypeName();
		
		const char* mapName = pRequest->GetMapName();
		if(mapName==NULL)
		{
			GError* pError = augeGetErrorInstance();
			pError->SetError("No Map is attached");
			return NULL;
		}

		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		Map *pMap = pCartoManager->LoadMap(mapName);
		if(pMap==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot load map [%s]", mapName);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return NULL;
		}

		pLayer = pMap->GetLayer(typeName);
		if(pLayer==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot Find %s has not FeatureType [%s]",pWebContext->GetService(), typeName);
			pLogger->Error(msg, __FILE__, __LINE__);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return NULL;
		}

		if(pLayer->GetType()!=augeLayerFeature)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "%s is not a Feature Layer",typeName);
			pLogger->Error(msg, __FILE__, __LINE__);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return NULL;
		}

		FeatureClass* pFeatureClass = NULL;
		FeatureLayer* pFLayer = static_cast<FeatureLayer*>(pLayer);
		pFeatureClass = pFLayer->GetFeatureClass();
		if(pFeatureClass==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot Get FeatureType %s",typeName);
			pLogger->Error(msg, __FILE__, __LINE__);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return NULL;
		}
		pFeatureClass->AddRef();
		return pFeatureClass;
	}

	FeatureClass* DescribeFeatureTypeHandler::GetFeatureClassBySource(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		const char* typeName = NULL;
		Layer* pLayer = NULL;
		GLogger *pLogger = augeGetLoggerInstance();
		DescribeFeatureTypeRequest* pRequest = static_cast<DescribeFeatureTypeRequest*>(pWebRequest);
		typeName = pRequest->GetTypeName();

		const char* sourceName = pRequest->GetSourceName();
		if(sourceName==NULL)
		{
			GError* pError = augeGetErrorInstance();
			pError->SetError("No Source is attached");
			return NULL;
		}

		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
		Workspace* pWorkspace = pConnManager->GetWorkspace(sourceName);
		if(pWorkspace==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "No DataSource Named [%s]", sourceName);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return NULL;
		}

		FeatureWorksapce* pFeatureWorkspace = dynamic_cast<FeatureWorksapce*>(pWorkspace);
		FeatureClass* pFeatureClass = pFeatureWorkspace->OpenFeatureClass(typeName);
		if(pFeatureClass==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot Get FeatureType %s",typeName);
			pLogger->Error(msg, __FILE__, __LINE__);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return NULL;
		}
		return pFeatureClass;
	}

	//WebResponse* DescribeFeatureTypeHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	//{
	//	DescribeFeatureTypeRequest* pRequest = static_cast<DescribeFeatureTypeRequest*>(pWebRequest);

	//	const char* typeName = NULL;
	//	Layer* pLayer = NULL;
	//	GLogger *pLogger = augeGetLoggerInstance();
	//	typeName = pRequest->GetTypeName();

	//	//if(pMap==NULL)
	//	//{
	//	//	char msg[AUGE_MSG_MAX];
	//	//	g_sprintf(msg, "FeatureType [%s] doesn't exist.",typeName);
	//	//	pLogger->Error(msg, __FILE__, __LINE__);

	//	//	WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
	//	//	pExpResopnse->SetMessage(msg);
	//	//	return pExpResopnse;
	//	//}

	//	pLayer = pMap->GetLayer(typeName);
	//	if(pLayer==NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "Service %s has not FeatureType %s,",pWebContext->GetService(), typeName);
	//		pLogger->Error(msg, __FILE__, __LINE__);

	//		WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
	//		pExpResopnse->SetMessage(msg);
	//		return pExpResopnse;
	//	}

	//	if(pLayer->GetType()!=augeLayerFeature)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "%s is not a Feature Layer",typeName);
	//		pLogger->Error(msg, __FILE__, __LINE__);

	//		WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
	//		pExpResopnse->SetMessage(msg);
	//		return pExpResopnse;
	//	}

	//	FeatureClass* pFeatureClass = NULL;
	//	FeatureLayer* pFLayer = static_cast<FeatureLayer*>(pLayer);
	//	pFeatureClass = pFLayer->GetFeatureClass();
	//	if(pFeatureClass==NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "Cannot Get FeatureType %s",typeName);
	//		pLogger->Error(msg, __FILE__, __LINE__);

	//		WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
	//		pExpResopnse->SetMessage(msg);
	//		return pExpResopnse;
	//	}

	//	char cache_name[AUGE_NAME_MAX];
	//	char cache_file[AUGE_PATH_MAX];
	//	const char* cache_path = pWebContext->GetCacheProtocolPath();
	//	g_snprintf(cache_name, AUGE_NAME_MAX,"%s_wfs_%s_describe_feauretype_1_1_0", pWebContext->GetService(), typeName);
	//	auge_make_path(cache_file, NULL, cache_path, cache_name,"xml");

	//	//if(access(capa_path,4))
	//	{
	//		WriteDescribeFeatureType(pRequest->GetVersion(), pWebContext, typeName, pFeatureClass);
	//	}

	//	DescribeFeatureTypeResponse* pResponse = new DescribeFeatureTypeResponse(pRequest);		
	//	pResponse->SetPath(cache_file);
	//	
	//	return pResponse;
	//}
	
	bool DescribeFeatureTypeHandler::WriteDescribeFeatureType(const char* version,WebContext* pWebContext, const char* typeName, FeatureClass* pFeatureClass)
	{
		if(strcmp(version, "1.0.0")==0)
		{
			return WriteDescribeFeatureType_1_1_0(pWebContext, typeName, pFeatureClass);
		}
		else if(strcmp(version, "1.1.0")==0)
		{
			return WriteDescribeFeatureType_1_1_0(pWebContext, typeName, pFeatureClass);
		}
		return false;
	}

	bool DescribeFeatureTypeHandler::WriteDescribeFeatureType_1_0_0(WebContext* pWebContext, const char* typeName, FeatureClass* pFeatureClass)
	{
		return WriteDescribeFeatureType_1_1_0(pWebContext, typeName, pFeatureClass);
		//char str[AUGE_NAME_MAX];
		//char cache_name[AUGE_NAME_MAX];
		//char cache_file[AUGE_PATH_MAX];
		//const char* cache_path = pWebContext->GetCacheProtocolPath();
		//g_snprintf(cache_name, AUGE_NAME_MAX,"%s_wfs_%s_describe_feauretype_1_1_0", pWebContext->GetService(), typeName);
		//auge_make_path(cache_file, NULL, cache_path, cache_name,"xml");

		//XElement  *pxNode = NULL;
		//XElement  *pxRoot = NULL;
		//XDocument *pxDoc = new XDocument();
		//pxRoot = pxDoc->CreateRootNode("schema", NULL, NULL);
		//pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs",NULL);
		//pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs","wfs");
		//pxRoot->SetNamespaceDeclaration("http://www.opengis.net/ows","ows");
		//pxRoot->SetNamespaceDeclaration("http://www.opengis.net/gml","gml");
		//pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		//pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		//pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema","xsd");		
		//pxRoot->SetAttribute("version", "1.1.0", NULL);

		//// schema-->import
		//XElement* pxImport = pxRoot->AddChild("import", "xsd");
		//pxImport->SetAttribute("namespace","http://www.opengis.net/gml", NULL);
		//// schema-->complexType
		//g_sprintf(str,"%sType", typeName);
		//XElement* pxComplexType = pxRoot->AddChild("complexType", "xsd");
		//pxComplexType->SetAttribute("name", str, NULL);
		//// schema-->complexType-->complexContent
		//XElement* pxComplexContent = pxComplexType->AddChild("complexContent","xsd");
		//// schema-->complexType-->complexContent-->extentsion
		//XElement* pxExtentsion = pxComplexContent->AddChild("extentsion", "xsd");
		//pxExtentsion->SetAttribute("base","gml:AbstractFeatureType", NULL);
		//// schema-->complexType-->complexContent-->extentsion-->sequence
		//XElement* pxSequence = pxExtentsion->AddChild("sequence", "xsd");

		//GField	*pField  = NULL;
		//GFields	*pFields = pFeatureClass->GetFields();
		//g_uint count = pFields->Count();

		//// schema-->complexType-->complexContent-->extentsion-->sequence-->element
		//XElement* pxElement = NULL;
		//for(g_uint i=0; i<count; i++)
		//{
		//	pField = pFields->GetField(i);
		//	pxElement = pxSequence->AddChild("element", "xsd");		
		//	pxElement->SetAttribute("name",pField->GetName(), NULL);
		//	pxElement->SetAttribute("nillable",pField->IsNullable()?"true":"false", NULL);
		//	pxElement->SetAttribute("minOccurs","0", NULL);
		//	pxElement->SetAttribute("maxOccurs","1", NULL);

		//	if(pField->GetType()==augeFieldTypeGeometry)
		//	{
		//		augeGeometryType gtype = pField->GetGeometryDef()->GeometryType();
		//		const char* sss = GetOgcGeometryType(gtype);
		//		g_sprintf(str,"gml:%sPropertyType",sss);
		//		pxElement->SetAttribute("type",str, NULL);
		//	}
		//	else
		//	{
		//		g_sprintf(str, "xsd:%s", GetOgcFieldType(pField->GetType()));
		//		pxElement->SetAttribute("type",str, NULL);
		//	}

		//}

		//// schema-->element
		//pxElement = pxRoot->AddChild("element", "xsd");
		//g_sprintf(str, "%sType", typeName);
		//pxElement->SetAttribute("type",str, NULL);
		//pxElement->SetAttribute("name",typeName, NULL);
		//pxElement->SetAttribute("substitutionGroup","gml:_Feature", NULL);

		//pxDoc->Save(cache_file, pWebContext->GetResponseEncoding(), 1);
		//pxDoc->Release();

		//return true;
	}

	bool DescribeFeatureTypeHandler::WriteDescribeFeatureType_1_1_0(WebContext* pWebContext, const char* typeName, FeatureClass* pFeatureClass)
	{
		char str[AUGE_NAME_MAX];
		char cache_name[AUGE_NAME_MAX];
		char cache_file[AUGE_PATH_MAX];
		const char* cache_path = pWebContext->GetCacheProtocolPath();
		g_snprintf(cache_name, AUGE_NAME_MAX,"%s_wfs_%s_describe_feauretype_1_1_0", pWebContext->GetService(), typeName);
		auge_make_path(cache_file, NULL, cache_path, cache_name,"xml");

		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		XDocument *pxDoc = new XDocument();
		pxRoot = pxDoc->CreateRootNode("schema", NULL, NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs",NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs","wfs");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/ows","ows");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/gml","gml");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema","xsd");		
		pxRoot->SetAttribute("version", "1.1.0", NULL);

		// schema-->import
		XElement* pxImport = pxRoot->AddChild("import", "xsd");
		pxImport->SetAttribute("namespace","http://www.opengis.net/gml", NULL);
		// schema-->complexType
		g_sprintf(str,"%sType", typeName);
		XElement* pxComplexType = pxRoot->AddChild("complexType", "xsd");
		pxComplexType->SetAttribute("name", str, NULL);
		// schema-->complexType-->complexContent
		XElement* pxComplexContent = pxComplexType->AddChild("complexContent","xsd");
		// schema-->complexType-->complexContent-->extentsion
		XElement* pxExtentsion = pxComplexContent->AddChild("extentsion", "xsd");
		pxExtentsion->SetAttribute("base","gml:AbstractFeatureType", NULL);
		// schema-->complexType-->complexContent-->extentsion-->sequence
		XElement* pxSequence = pxExtentsion->AddChild("sequence", "xsd");

		GField	*pField  = NULL;
		GFields	*pFields = pFeatureClass->GetFields();
		g_uint count = pFields->Count();

		// schema-->complexType-->complexContent-->extentsion-->sequence-->element
		XElement* pxElement = NULL;
		for(g_uint i=0; i<count; i++)
		{
			pField = pFields->GetField(i);
			pxElement = pxSequence->AddChild("element", "xsd");
			const char* fname = pField->GetName();
			const char* utf8_name = auge_encoding_convert("GBK","UTF-8",fname,strlen(fname));
			pxElement->SetAttribute("name",utf8_name, NULL);
			pxElement->SetAttribute("nillable",pField->IsNullable()?"true":"false", NULL);
			pxElement->SetAttribute("minOccurs","0", NULL);
			pxElement->SetAttribute("maxOccurs","1", NULL);

			if(pField->GetType()==augeFieldTypeGeometry)
			{
				augeGeometryType gtype = pField->GetGeometryDef()->GeometryType();
				const char* sss = GetOgcGeometryType(gtype);
				g_sprintf(str,"gml:%sPropertyType",sss);
				pxElement->SetAttribute("type",str, NULL);
			}
			else
			{
				g_sprintf(str, "xsd:%s", GetOgcFieldType(pField->GetType()));
				pxElement->SetAttribute("type",str, NULL);
			}

		}

		// schema-->element
		pxElement = pxRoot->AddChild("element", "xsd");
		g_sprintf(str, "%sType", typeName);
		pxElement->SetAttribute("type",str, NULL);
		pxElement->SetAttribute("name",typeName, NULL);
		pxElement->SetAttribute("substitutionGroup","gml:_Feature", NULL);

		pxDoc->Save(cache_file, pWebContext->GetResponseEncoding(), 1);
		pxDoc->Release();

		return true;
	}

	//bool DescribeFeatureTypeHandler::AddFeatureTypeNode(WebContext* pWebContext, const char* typeName, FeatureClass* pFeatureClass)
	//{
	//	char str[AUGE_NAME_MAX];
	//	char cache_name[AUGE_NAME_MAX];
	//	char cache_file[AUGE_PATH_MAX];
	//	const char* cache_path = pWebContext->GetCacheProtocolPath();
	//	g_snprintf(cache_name, AUGE_NAME_MAX,"%s_wfs_%s_describe_feauretype_1_1_0", pWebContext->GetService(), typeName);
	//	auge_make_path(cache_file, NULL, cache_path, cache_name,"xml");

	//	XElement  *pxNode = NULL;
	//	XElement  *pxRoot = NULL;
	//	XDocument *pxDoc = new XDocument();
	//	pxRoot = pxDoc->CreateRootNode("schema", NULL, NULL);
	//	pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs",NULL);
	//	pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs","wfs");
	//	pxRoot->SetNamespaceDeclaration("http://www.opengis.net/ows","ows");
	//	pxRoot->SetNamespaceDeclaration("http://www.opengis.net/gml","gml");
	//	pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
	//	pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
	//	pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema","xsd");		
	//	pxRoot->SetAttribute("version", "1.1.0", NULL);

	//	// schema-->import
	//	XElement* pxImport = pxRoot->AddChild("import", "xsd");
	//	pxImport->SetAttribute("namespace","http://www.opengis.net/gml", NULL);
	//	// schema-->complexType
	//	g_sprintf(str,"%sType", typeName);
	//	XElement* pxComplexType = pxRoot->AddChild("complexType", "xsd");
	//	pxComplexType->SetAttribute("name", str, NULL);
	//	// schema-->complexType-->complexContent
	//	XElement* pxComplexContent = pxComplexType->AddChild("complexContent","xsd");
	//	// schema-->complexType-->complexContent-->extentsion
	//	XElement* pxExtentsion = pxComplexContent->AddChild("extentsion", "xsd");
	//	// schema-->complexType-->complexContent-->extentsion-->sequence
	//	XElement* pxSequence = pxExtentsion->AddChild("extentsion", "xsd");
	//	pxSequence->SetAttribute("base","gml:AbstractFeatureType", NULL);

	//	GField	*pField  = NULL;
	//	GFields	*pFields = pFeatureClass->GetFields();
	//	g_uint count = pFields->Count();

	//	// schema-->complexType-->complexContent-->extentsion-->sequence-->element
	//	XElement* pxElement = NULL;
	//	for(g_uint i=0; i<count; i++)
	//	{
	//		pField = pFields->GetField(i);
	//		pxElement = pxSequence->AddChild("element", "xsd");		
	//		pxElement->SetAttribute("name",pField->GetName(), NULL);
	//		pxElement->SetAttribute("nillable",pField->IsNullable()?"true":"false", NULL);
	//		pxElement->SetAttribute("minOccurs","0", NULL);
	//		pxElement->SetAttribute("maxOccurs","1", NULL);

	//		if(pField->GetType()==augeFieldTypeGeometry)
	//		{
	//			augeGeometryType gtype = pField->GetGeometryDef()->GeometryType();
	//			const char* sss = GetOgcGeometryType(gtype);
	//			g_sprintf(str,"gml:%sPropertyType",sss);
	//			pxElement->SetAttribute("type",str, NULL);
	//		}
	//		else
	//		{
	//			g_sprintf(str, "xsd:%s", GetOgcFieldType(pField->GetType()));
	//			pxElement->SetAttribute("type",str, NULL); 
	//		}

	//	}

	//	// schema-->element
	//	pxElement = pxRoot->AddChild("element", "xsd");
	//	g_sprintf(str, "%sType", typeName);
	//	pxElement->SetAttribute("type",str, NULL);
	//	pxElement->SetAttribute("name",typeName, NULL);
	//	pxElement->SetAttribute("substitutionGroup","gml:_Feature", NULL);

	//	pxDoc->Save(cache_file, pWebContext->GetResponseEncoding(), 1);
	//	pxDoc->Release();

	//	return true;
	//}


	const char* DescribeFeatureTypeHandler::GetOgcFieldType(augeFieldType type)
	{
		switch(type)
		{
		case augeFieldTypeBool:
			return "boolean";
		case augeFieldTypeChar:
			return "character";
		case augeFieldTypeShort:
			return "short";
		case augeFieldTypeInt:
			return "int";
		case augeFieldTypeLong:
			return "long";
		case augeFieldTypeInt64:
			return "int64";
		case augeFieldTypeFloat:
			return "float";
		case augeFieldTypeDouble:
			return "double";
		case augeFieldTypeString:
			return "string";
		case augeFieldTypeTime:
			return "timestamp";
		case augeFieldTypeBLOB:
			return "blob";
		case augeFieldTypeGeometry:
			return "geometry";
		case augeFieldTypeCLOB:
			return "clob";
		case augeFieldTypeNCLOB:
			return "nclob";
		case augeFieldTypeSerial:
			return "serial";
		}
		return "";
	}

	const char*	DescribeFeatureTypeHandler::GetOgcGeometryType(augeGeometryType type)
	{
		switch(type)
		{
		case augeGTPoint:
			return "Point";
		case augeGTLineString:
			return "LineString";
		case augeGTPolygon:
			return "Polygon";
		case augeGTMultiPoint:
			return "MultiPoint";
		case augeGTMultiLineString:
			return "MultiLineString";
		case augeGTMultiPolygon:
			return "MultiPolygon";
		}
		return "";
	}
}
