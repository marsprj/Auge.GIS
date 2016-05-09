#ifndef __AUGE_IMS_GET_JOB_RESPONSE_H__
#define __AUGE_IMS_GET_JOB_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class Job;
	class EnumJob;
	class GetJobRequest;

	class GetJobResponse : public WebResponse
	{
	public:
		GetJobResponse(GetJobRequest* pRequest);
		virtual ~GetJobResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
		
	public:
		void	SetJob(Job* pJob);
		void	SetJobs(EnumJob* pJobs);
		void	SetWebContext(WebContext* pWebContext);

	private:
		void	WriteJob(XDocument* pxDoc);
		void	WriteJobs(XDocument* pxDoc);

		void	AddJobNode(XElement* pxRoot, Job* pJob); 
		//void	AddJobNode(XDocument* pxDoc, Job* pJob);
		//void	AddLayerNode(XElement* pxLayers, Layer* pLayer);
		//void	AddBoundingBoxNode(XElement* pxRoot, GEnvelope& extent);



		//void	AddLayerGeomTypeNode(XElement* pxLayer, FeatureLayer* pFeatureLayer);
		//void	AddLayerGeographicBoundingNode(XElement* pxLayer, GEnvelope& extent);
		//void	AddLayerBoundingNode(XElement* pxParent, GEnvelope& extent, int srid);
		//void	AddViewerNode(XElement* pxParent, GEnvelope& extent, int srid);
		//void	AddStyleNode(XElement* pxParent, Style* pStyle);

		//void	AddWebURLNode(XElement* pxLayer, const char* url);
		//void	AddThumbnailNode(XElement* pxThumbnail, Job* pJob);

		void	Cleanup();
		
	private:
		GetJobRequest	*m_pRequest;
		WebContext	*m_pWebContext;
		EnumJob		*m_pJobs;
		Job			*m_pJob;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
