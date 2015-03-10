#ifndef __AUGE_IMS_SERVICE_REMOVE_REQUEST_H__
#define __AUGE_IMS_SERVICE_REMOVE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class RemoveMapRequest : public WebRequest
	{
	public:
		RemoveMapRequest();
		virtual ~RemoveMapRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();

		virtual const char*		GetName();
	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* value);
		void		SetName(const char* name);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_name;
	};
}

#endif //__AUGE_IMS_SERVICE_REMOVE_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=ims&version=1.0.0&request=RemoveMap&name=world

[ HTTP Post ]
-------------------------------------------------------------------------
<?xml version="1.0"?>
<RemoveMap>
	<Name>world</Name>
</RemoveMap>

************************************************************************/