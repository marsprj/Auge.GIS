#ifndef __AUGE_WMS_GETCAPABILITIES_REQUEST_H__
#define __AUGE_WMS_GETCAPABILITIES_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>

namespace auge
{
	class GetCapabilitiesRequest : public WebRequest
	{
	public:
		GetCapabilitiesRequest();
		virtual ~GetCapabilitiesRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();

	public:
		void		SetVersion(const char* value);
		bool		Create(GPropertySet& props);

	private:
		std::string m_version;
		std::string m_mime_type;
	};
}

//"service=wms&version=1.3.0&request=GetMap&layers=cities,rivers,country&styles=point,line,polygon&bbox=-180,-90,180,90&width=800&height=600&transparent=true";

#endif //__AUGE_WMS_GETCAPABILITIES_REQUEST_H__
