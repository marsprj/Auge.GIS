#ifndef __AUGE_IMS_CAPABILITIES_REQUEST_H__
#define __AUGE_IMS_CAPABILITIES_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class CapabilitiesRequest : public WebRequest
	{
	public:
		CapabilitiesRequest();
		virtual ~CapabilitiesRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();

	public:
		void		SetVersion(const char* value);
		bool		Create(rude::CGI& cgi);

	private:
		std::string m_version;
		std::string m_mime_type;

		std::string m_name;
	};

}

#endif //__AUGE_IMS_CAPABILITIES_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=ims&version=1.0.0&request=GetCapabilities

************************************************************************/