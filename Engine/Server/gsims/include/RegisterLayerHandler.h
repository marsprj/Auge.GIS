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
	class RegisterLayerRequest;

	class RegisterLayerHandler : public WebHandler
	{
	public:
		RegisterLayerHandler();
		virtual ~RegisterLayerHandler();
	public:
		virtual const char*		GetName();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext);

	private:
		WebResponse*	RegisterFeatureLayer(RegisterLayerRequest* pRequest, Map* pMap);
		WebResponse*	RegisterQuadServerLayer(RegisterLayerRequest* pRequest, Map* pMap);
	};
}

#endif //__AUGE_IMS_REGISTER_LAYER_HANDLER_H__
