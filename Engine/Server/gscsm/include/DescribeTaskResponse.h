#ifndef __AUGE_CSM_DESCRIBE_TASK_RESPONSE_H__
#define __AUGE_CSM_DESCRIBE_TASK_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class Map;
	class Layer;
	class EnumMap;
	class FeatureLayer;
	class DescribeTaskRequest;
	class Style;

	class EnumTask;
	class CsTask;

	class DescribeTaskResponse : public WebResponse
	{
	public:
		DescribeTaskResponse(DescribeTaskRequest* pRequest);
		virtual ~DescribeTaskResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
		
	public:
		void	SetTask(CsTask* pTask);
		void	SetTasks(EnumTask* pTasks);
		void	SetWebContext(WebContext* pWebContext);

	private:
		void	AddTaskNode(XElement* pxRoot, CsTask* pTask); 
		void	AddMapNode(XDocument* pxDoc, Map* pMap);
		void	AddLayerNode(XElement* pxLayers, Layer* pLayer);
		void	AddBoundingBoxNode(XElement* pxRoot, GEnvelope& extent);

		void	WriteMap(XDocument* pxDoc);
		void	WriteTasks(XDocument* pxDoc);

		void	AddLayerGeomTypeNode(XElement* pxLayer, FeatureLayer* pFeatureLayer);
		void	AddLayerGeographicBoundingNode(XElement* pxLayer, GEnvelope& extent);
		void	AddLayerBoundingNode(XElement* pxParent, GEnvelope& extent, int srid);
		void	AddViewerNode(XElement* pxParent, GEnvelope& extent, int srid);
		void	AddStyleNode(XElement* pxParent, Style* pStyle);

		void	AddWebURLNode(XElement* pxLayer, const char* url);
		void	AddThumbnailNode(XElement* pxThumbnail, Map* pMap);

		void	Cleanup();
		
	private:
		DescribeTaskRequest	*m_pRequest;
		WebContext	*m_pWebContext;
		//EnumMap		*m_pMaps;
		CsTask			*m_pTask;

		EnumTask	*m_pTasks;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
