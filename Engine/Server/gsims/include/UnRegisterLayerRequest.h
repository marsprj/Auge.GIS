#ifndef __AUGE_IMS_UNREGISTER_LAYER_REQUEST_H__
#define __AUGE_IMS_UNREGISTER_LAYER_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class UnRegisterLayerRequest : public WebRequest
	{
	public:
		UnRegisterLayerRequest();
		virtual ~UnRegisterLayerRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

	public:
		bool		Create(rude::CGI& cgi);

		const char*	GetName();
		const char*	GetMapName();
		const char* GetLayerName();

		void		SetVersion(const char* value);
		void		SetMapName(const char* mapName);
		void		SetLayerName(const char* layerName);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_map_name;
		std::string m_layer_name;
		std::string m_encoding;
	};
}

#endif //__AUGE_IMS_UNREGISTER_LAYER_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=ims&version=1.0.0&request=UnRegisterLayer&mapName=world&layerName=cities
http://127.0.0.1:8088/ows/user1/mgr?service=ims&version=1.0.0&request=UnRegisterLayer&mapName=world&&layerName=cities
http://192.168.111.160:8088/ows/user1/mgr?service=ims&version=1.0.0&request=UnRegisterLayer&mapName=world&&layerName=cities

************************************************************************/