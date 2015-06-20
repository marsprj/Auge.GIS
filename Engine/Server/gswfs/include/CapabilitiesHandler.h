#ifndef __AUGE_WFS_CAPABILITIES_HANDLER_H__
#define __AUGE_WFS_CAPABILITIES_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{ 
	class Map;
	class WebContext;
	class FeatureClass;
	class AttributeDataSet;
	class Workspace;
	class CapabilitiesRequest;
	class CapabilitiesResponse;

	class CapabilitiesHandler : public WebHandler
	{
	public:
		CapabilitiesHandler();
		virtual ~CapabilitiesHandler();
	public:
		virtual const char*		GetName();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		//virtual WebRequest*		ParseRequest(rude::CGI& cgi, WebContext* pWebContext=NULL, Map* pMap=NULL);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);
		//virtual WebRequest*		ParseRequest(XDocument* pxDoc, WebContext* pWebContext=NULL, Map* pMap=NULL);

		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext);
		//virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap);
	private:
		WebResponse*			ExecuteByMap(CapabilitiesRequest* pWebRequest, WebContext* pWebContext);
		CapabilitiesResponse*	WriteCapabilities_1_0_0(CapabilitiesRequest* pRequest, WebContext* pWebContext, Map* pMap);
		CapabilitiesResponse*	WriteCapabilities_1_1_0(CapabilitiesRequest* pRequest, WebContext* pWebContext, Map* pMap);

		CapabilitiesResponse*	WriteCapabilities_1_0_0(CapabilitiesRequest* pRequest, WebContext* pWebContext, Workspace* pWorkspace);
		CapabilitiesResponse*	WriteCapabilities_1_1_0(CapabilitiesRequest* pRequest, WebContext* pWebContext, Workspace* pWorkspace);


		void					SetRooteNode_1_1_0(XElement* pxRoot, const char* version);
		void					AddServiceIdentificationNode_1_1_0(XElement* pxParent);
		void					AddServiceProviderNode_1_1_0(XElement* pxParent);
		void					AddOperationsMetadataNode_1_1_0(XElement* pxParent, const char* wfs_xlink);
		XElement*				AddFeatureTypeListNode_1_1_0(XElement* pxParent);
		void					AddFeatureTypeNode_1_1_0(XElement* pxParent, const char* typeName, FeatureClass* pFeatureClass);
		void					AddAttributeDatasetsNode_1_1_0(XElement* pxParent, const char* name, AttributeDataSet* pattrDataset);
		

		WebResponse*			ExecuteBySource(CapabilitiesRequest* pWebRequest, WebContext* pWebContext);
	};
}

#endif //__AUGE_WFS_CAPABILITIES_HANDLER_H__

