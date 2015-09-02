#include "GetJobRequest.h"
#include "GetJobResponse.h"

namespace auge
{ 
	GetJobResponse::GetJobResponse(GetJobRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_pJob = NULL;
		m_pJobs = NULL;
	}

	GetJobResponse::~GetJobResponse()
	{
		Cleanup();
	}

	void GetJobResponse::Cleanup()
	{
		if(m_pJobs!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pJobs);
		}
		if(m_pJob!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pJob);
		}
	}

	void GetJobResponse::SetJob(Job* pJob)
	{
		Cleanup();
		m_pJob = pJob;
	}

	void GetJobResponse::SetJobs(EnumJob* pJobs)
	{
		Cleanup();
		m_pJobs = pJobs;
	}

	void GetJobResponse::SetWebContext(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}

	RESULTCODE GetJobResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		XDocument* pxDoc = new XDocument();
		if(m_pJobs!=NULL)
		{
			WriteJobs(pxDoc);
		}
		else if(m_pJob)
		{
			WriteJob(pxDoc);
		}

		int len = 0;
		g_uchar* buffer = NULL;
		GLogger* pLogger = augeGetLoggerInstance();

		pxDoc->WriteToString(&buffer, len, m_pRequest->GetEncoding(),1);
		
		pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		pWriter->Write(buffer, len);
		pWriter->WriteTail();

		pLogger->Info((g_char*)buffer);

		return AG_SUCCESS;
	}

	void GetJobResponse::WriteJob(XDocument* pxDoc)
	{
		GLogger* pLogger = augeGetLoggerInstance();

		Job* pJob = NULL;
		JobManager* pJobManager = augeGetJobManagerInstance();

		XElement	*pxNode = NULL;

		XElement	*pxRoot = pxDoc->CreateRootNode("Jobs", NULL, NULL);
		if(m_pJob!=NULL)
		{
			AddJobNode(pxRoot, m_pJob);
		}
	}

	////////////////////////////////////////////////////////////////////////////
	//// Add Job
	////////////////////////////////////////////////////////////////////////////
	////void GetJobResponse::SetLayerNode(XElement* pxLayer, const char* name, const char* title, const char* abstract, g_int srid, bool queryable)
	////{
	////	return NULL;
	////}
	// 
	//void GetJobResponse::AddStyleNode(XElement* pxParent, Style* pStyle)
	//{
	//	char str[AUGE_NAME_MAX];
	//	XElement* pxStyle = pxParent->AddChild("Style", NULL);
	//	g_sprintf(str, "%d", pStyle->GetID());
	//	pxStyle->SetAttribute("id",str,NULL);

	//	XElement* pxNode = pxStyle->AddChild("Name",NULL);
	//	pxNode->AddChildText(pStyle->GetName());
	//	pxNode = pxStyle->AddChild("Title",NULL);
	//	pxNode = pxStyle->AddChild("Abstract",NULL);
	//	XElement* pxLegendURL = pxStyle->AddChild("LegendURL",NULL);
	//	pxLegendURL->SetAttribute("width","20", NULL);
	//	pxLegendURL->SetAttribute("height","20", NULL);
	//	pxNode = pxLegendURL->AddChild("Format",NULL);
	//	pxNode->SetChildText("image/png");
	//	pxNode = pxLegendURL->AddChild("OnlineResource",NULL);
	//}

	//void GetJobResponse::AddLayerBoundingNode(XElement* pxParent, GEnvelope& extent, int srid)
	//{
	//	XElement* pxNode = NULL;
	//	XElement* pxBounding = NULL;
	//	char str[AUGE_NAME_MAX];

	//	pxBounding = pxParent->AddChild("BoundingBox", NULL);
	//	g_sprintf(str, "CRS:%d", srid);
	//	pxBounding->SetAttribute("CRS", str,NULL);

	//	if(!extent.IsValid())
	//	{
	//		return;
	//	}
	//	
	//	g_sprintf(str, "%.6f", extent.m_xmin);
	//	pxBounding->SetAttribute("minx", str,NULL);
	//	g_sprintf(str, "%.6f", extent.m_xmax);
	//	pxBounding->SetAttribute("maxx", str,NULL);
	//	g_sprintf(str, "%.6f", extent.m_ymin);
	//	pxBounding->SetAttribute("miny", str,NULL);		
	//	g_sprintf(str, "%.6f", extent.m_ymax);
	//	pxBounding->SetAttribute("maxy", str,NULL);
	//}

	//void GetJobResponse::AddViewerNode(XElement* pxParent, GEnvelope& extent, int srid)
	//{
	//	XElement* pxNode = NULL;
	//	XElement* pxBounding = NULL;
	//	char str[AUGE_NAME_MAX];
	//	pxBounding = pxParent->AddChild("Viewer", NULL);
	//	g_sprintf(str, "CRS:%d", srid);
	//	pxBounding->SetAttribute("CRS", str,NULL);

	//	if(!extent.IsValid())
	//	{
	//		return;
	//	}
	//	
	//	g_sprintf(str, "%.6f", extent.m_xmin);
	//	pxBounding->SetAttribute("minx", str,NULL);
	//	g_sprintf(str, "%.6f", extent.m_xmax);
	//	pxBounding->SetAttribute("maxx", str,NULL);
	//	g_sprintf(str, "%.6f", extent.m_ymin);
	//	pxBounding->SetAttribute("miny", str,NULL);		
	//	g_sprintf(str, "%.6f", extent.m_ymax);
	//	pxBounding->SetAttribute("maxy", str,NULL);
	//}

	//void GetJobResponse::AddLayerGeographicBoundingNode(XElement* pxParent, GEnvelope& extent)
	//{
	//	XElement* pxNode = NULL;
	//	XElement* pxGeoBounding = NULL;
	//	char str[AUGE_NAME_MAX];

	//	pxGeoBounding = pxParent->AddChild("EX_GeographicBoundingBox", NULL);
	//	g_sprintf(str, "%.6f", extent.m_xmin);
	//	pxNode = pxGeoBounding->AddChild("westBoundLongitude");
	//	pxNode->SetChildText(str);
	//	g_sprintf(str, "%.6f", extent.m_xmax);
	//	pxNode = pxGeoBounding->AddChild("eastBoundLongitude");
	//	pxNode->SetChildText(str);
	//	g_sprintf(str, "%.6f", extent.m_ymin);
	//	pxNode = pxGeoBounding->AddChild("southBoundLatitude");
	//	pxNode->SetChildText(str);
	//	g_sprintf(str, "%.6f", extent.m_ymax);
	//	pxNode = pxGeoBounding->AddChild("northBoundLatitude");
	//	pxNode->SetChildText(str);
	//}

	//void GetJobResponse::AddLayerGeomTypeNode(XElement* pxLayer, FeatureLayer* pFeatureLayer)
	//{
	//	XElement* pxGeomType = pxLayer->AddChild("GeometryType", NULL);
	//	FeatureClass* pFeatureClass = pFeatureLayer->GetFeatureClass();
	//	if(pFeatureClass!=NULL)
	//	{
	//		GField* pField = pFeatureClass->GetFields()->GetGeometryField();
	//		if(pField!=NULL)
	//		{
	//			GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
	//			const char* type = pGeometryFactory->Encode(pField->GetGeometryDef()->GeometryType());
	//			pxGeomType->AddChildText(type);
	//		}
	//	}
	//}
	//
	////////////////////////////////////////////////////////////////////////////
	//// Add Jobs
	////////////////////////////////////////////////////////////////////////////
	void GetJobResponse::WriteJobs(XDocument* pxDoc)
	{
		GLogger* pLogger = augeGetLoggerInstance();

		Job* pJob = NULL;
		JobManager* pJobManager = augeGetJobManagerInstance();

		XElement	*pxNode = NULL;
		
		XElement	*pxRoot = pxDoc->CreateRootNode("Jobs", NULL, NULL);
		m_pJobs->Reset();
		while((pJob=m_pJobs->Next())!=NULL)
		{
			AddJobNode(pxRoot, pJob);
		}
	}

	void GetJobResponse::AddJobNode(XElement* pxRoot, Job* pJob)
	{
		char str[AUGE_MSG_MAX];

		XElement *pxNode = NULL;
		XElement *pxExtent = NULL;
		XElement *pxJob  = pxRoot->AddChild("Job", NULL);
		pxNode = pxJob->AddChild("UUID");
		pxNode->AddChildText(pJob->GetUUID());

		pxNode = pxJob->AddChild("Operation");
		pxNode->AddChildText(pJob->GetOperation());

		pxNode = pxJob->AddChild("Params");
		pxNode->AddChildText(pJob->GetParams());
	
		pxNode = pxJob->AddChild("Client");
		pxNode->AddChildText(pJob->GetClient());

		pxNode = pxJob->AddChild("Server");
		pxNode->AddChildText(pJob->GetServer());

		TIME_STRU start_stru;
		pJob->GetStartTime(&start_stru);
		char time_str[AUGE_NAME_MAX];
		memset(time_str, 0, AUGE_NAME_MAX);
		g_snprintf(time_str,AUGE_NAME_MAX,"%d-%02d-%02d %02d:%02d:%02d",start_stru.usYear,
			start_stru.usMonth,
			start_stru.usDay,
			start_stru.usHour,
			start_stru.usMinute,
			start_stru.usSecond);
		pxNode = pxJob->AddChild("StartTime");
		pxNode->AddChildText(time_str);

		pJob->GetEndTime(&start_stru);
		memset(time_str, 0, AUGE_NAME_MAX);
		g_snprintf(time_str,AUGE_NAME_MAX,"%d-%02d-%02d %02d:%02d:%02d",start_stru.usYear,
			start_stru.usMonth,
			start_stru.usDay,
			start_stru.usHour,
			start_stru.usMinute,
			start_stru.usSecond);

		pxNode = pxJob->AddChild("EndTime");
		pxNode->AddChildText(time_str);

		pxNode = pxJob->AddChild("State");
		switch(pJob->GetState())
		{
		case augeStateRunning:
			pxNode->AddChildText("Running");
			break;
		case augeStateFinished:
			pxNode->AddChildText("Finished");
			break;
		case augeStateCanceled:
			pxNode->AddChildText("Canceled");
			break;
		}
	}

	//void GetJobResponse::AddThumbnailNode(XElement* pxThumbnail, Job* pJob)
	//{
	//	char thumbnail[AUGE_PATH_MAX];
	//	memset(thumbnail,0,AUGE_PATH_MAX);
	//	g_snprintf(thumbnail, AUGE_PATH_MAX, "http://%s:%s/ows/thumbnail/%s", m_pWebContext->GetServer(), m_pWebContext->GetPort(), pJob->GetThumbnail());
	//	pxThumbnail->SetAttribute("xlink",thumbnail,NULL);
	//}

	//void GetJobResponse::AddJobNode(XDocument* pxDoc, Job* pJob)
	//{
	//	char str[AUGE_MSG_MAX];

	//	XElement *pxNode = NULL;
	//	XElement *pxExtent = NULL;
	//	XElement *pxJob  = pxDoc->CreateRootNode("Job", NULL, NULL);
	//	pxNode = pxJob->AddChild("Name");
	//	pxNode->AddChildText(pJob->GetName());

	//	// SRID
	//	g_sprintf(str, "%d", pJob->GetSRID());
	//	pxNode = pxJob->AddChild("Srid");
	//	pxNode->AddChildText(str);

	//	// Extent
	//	AddBoundingBoxNode(pxJob, pJob->GetExtent());		

	//	// Layers
	//	XElement* pxLayers = pxJob->AddChild("Layers");

	//	Layer* pLayer = NULL;
	//	g_int count = pJob->GetLayerCount();
	//	for(g_int i=0; i<count; i++)
	//	{
	//		pLayer = pJob->GetLayer(i);
	//		if(pLayer!=NULL)
	//		{
	//			AddLayerNode(pxLayers, pLayer);
	//		}
	//	}
	//}

	//void GetJobResponse::AddBoundingBoxNode(XElement* pxRoot, GEnvelope& extent)
	//{
	//	XElement* pxNode = NULL;
	//	XElement* pxCorner = NULL;
	//	XElement* pxExtent = NULL;
	//	
	//	pxExtent = pxRoot->AddChild("BoundingBox");

	//	if(extent.IsValid())
	//	{
	//		char str[AUGE_MSG_MAX];
	//		g_sprintf(str, "%f %f", extent.m_xmin, extent.m_ymin);
	//		pxCorner = pxExtent->AddChild("LowerCorner");
	//		pxCorner->AddChildText(str);

	//		g_sprintf(str, "%f %f", extent.m_xmax, extent.m_ymax);
	//		pxCorner = pxExtent->AddChild("UpperCorner");
	//		pxCorner->AddChildText(str);
	//	}
	//	else
	//	{
	//		pxCorner = pxExtent->AddChild("LowerCorner");
	//		pxCorner->AddChildText("0 0");

	//		pxCorner = pxExtent->AddChild("UpperCorner");
	//		pxCorner->AddChildText("0 0");
	//	}		
	//}


	//void GetJobResponse::AddLayerNode(XElement* pxLayers, Layer* pLayer)
	//{

	//	XElement *pxNode = NULL;
	//	XElement *pxLayer = NULL;

	//	pxLayer = pxLayers->AddChild("Layer");
	//	pxNode = pxLayer->AddChild("Name");
	//	pxNode->AddChildText(pLayer->GetName());

	//	AddBoundingBoxNode(pxLayer, pLayer->GetExtent()); 
	//}

	//void GetJobResponse::AddWebURLNode(XElement* pxLayer, const char* url)
	//{
	//	XElement* pxNode = NULL;
	//	pxNode = pxLayer->AddChild("URL");
	//	pxNode->AddChildText(url);

	//}
}