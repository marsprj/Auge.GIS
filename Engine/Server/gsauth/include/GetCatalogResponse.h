#ifndef __AUGE_IMS_GET_CATALOG_RESPONSE_H__
#define __AUGE_IMS_GET_CATALOG_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class User;
	class GField;
	class GFields;
	class Workspace;
	class FeatureClass;
	class FeatureWorkspace;
	class FeatureLayer;
	class Style;
	class Service;

	class GetCatalogResponse : public WebResponse
	{
	public:
		GetCatalogResponse(GetCatalogRequest* pRequest);
		virtual ~GetCatalogResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	public:
		void		SetUser(User* pUser);

	private:
		XDocument*	BuildDocument();
		void		BuildDataNode(XElement* pxParent);
		void		BuildVectorDataNode(XElement* pxParent);
		void		BuildRasterDataNode(XElement* pxParent);
		void		BuildTileDataNode(XElement* pxParent);
		void		BuildMapsNode(XElement* pxParent);
		void		BuildServicesNode(XElement* pxParent);
		void		BuildOperatorsNode(XElement* pxParent);

		void		BuildDataSourceNode(XElement* pxParent, Workspace* pWorkspace);
		void		BuildMapNode(XElement* pxParent, Map* pMap);
		void		AddBoundingNode(XElement* pxParent, GEnvelope& extent, int srid);
		void		AddLayerGeomTypeNode(XElement* pxLayer, FeatureLayer* pFeatureLayer);
		void		AddStyleNode(XElement* pxParent, Style* pStyle);

		void		AddServiceNode(XElement* pxParent, Service* pService);
		void		AddOperatorNode(XElement* pxParent, const char* name, const char* description);

		// @deprecated
		void		BuildFeatureWorkspaceNode(XElement* pxParent, FeatureWorkspace* pFeatureWorkspace);
		// @deprecated
		void		BuildFeatureWorkspaceNode(XElement* pxParent, FeatureClass* pFeatureClass);
		// @deprecated
		void		BuildFeatureFieldNode(XElement* pxFields, GField* pField);

	private:
		User* m_pUser;
		GetCatalogRequest *m_pRequest;	
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
