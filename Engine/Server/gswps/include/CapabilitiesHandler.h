#ifndef __AUGE_WPS_CAPABILITIES_HANDLER_H__
#define __AUGE_WPS_CAPABILITIES_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class WebContext;
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
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext);

	private:
		CapabilitiesResponse*	WriteCapabilities_1_0_0(CapabilitiesRequest* pRequest, WebContext* pWebContext);
		CapabilitiesResponse*	WriteCapabilities_1_3_0(CapabilitiesRequest* pRequest, WebContext* pWebContext);

		//virtual WebRequest*	ParseRequest(rude::CGI& cgi, WebContext* pWebContext=NULL, Map* pMap=NULL);
		//virtual WebRequest*	ParseRequest(XDocument* pxDoc, WebContext* pWebContext=NULL, Map* pMap=NULL);
		//virtual WebResponse*	Execute(WebRequest* pWebRequest);
		//virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap);

	private:
		void					SetRooteNode_1_1_0(XElement* pxRoot, const char* version);
		void					AddServiceIdentificationNode_1_1_0(XElement* pxParent);
		void					AddServiceProviderNode_1_1_0(XElement* pxParent);
		void					AddOperationsMetadataNode_1_1_0(XElement* pxParent, const char* wfs_xlink);
		void					AddProcessOfferingsNode_1_1_0(XElement* pxParent, const char* wps_xlink);
		void					AddLanguagesNode_1_1_0(XElement* pxParent, const char* wps_xlink);

	protected:
		void*	m_handler;
		std::vector<WebHandler*>	m_handlers;
	};
}

#endif //__AUGE_WPS_CAPABILITIES_HANDLER_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=ims&version=1.0.0&request=GetCapabilities

************************************************************************/