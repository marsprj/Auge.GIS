#ifndef __AUGE_WFS_GETCAPABILITIES_REQUEST_H__
#define __AUGE_WFS_GETCAPABILITIES_REQUEST_H__

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

#endif //__AUGE_WFS_GETCAPABILITIES_REQUEST_H__
