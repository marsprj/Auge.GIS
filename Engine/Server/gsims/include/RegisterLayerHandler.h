#ifndef __AUGE_IMS_REGISTER_LAYER_HANDLER_H__
#define __AUGE_IMS_REGISTER_LAYER_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class Map;
	class User;
	class RegisterLayerRequest;

	class RegisterLayerHandler : public WebHandler
	{
	public:
		RegisterLayerHandler();
		virtual ~RegisterLayerHandler();
	public:
		virtual const char*		GetName();
		virtual const char*		GetDescription();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);

	private:
		WebResponse*	RegisterFeatureLayer(RegisterLayerRequest* pRequest, Map* pMap, User* pUser);
		WebResponse*	RegisterRasterLayer(RegisterLayerRequest* pRequest, Map* pMap, User* pUser);
		WebResponse*	RegisterQuadServerLayer(RegisterLayerRequest* pRequest, Map* pMap, User* pUser);
		WebResponse*	RegisterWMTSLayer(RegisterLayerRequest* pRequest, Map* pMap, User* pUser);
	};
}

#endif //__AUGE_IMS_REGISTER_LAYER_HANDLER_H__
