#ifndef __AUGE_WMTS_REQUEST_H__
#define __AUGE_WMTS_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class WTileRequest : public WebRequest
	{
	public:
		WTileRequest();
		virtual ~WTileRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetUser();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetMapName();
		virtual const char*		GetEncoding();

		virtual const char*		GetHost();
		virtual const char*		GetRequestMethod();

	public:
		void		SetVersion(const char* value);
		void		SetUser(const char* user);
		void		SetMapName(const char* mapName);

		void		SetHost(const char* host);
		void		SetRequestMethod(const char* method);

	protected:
		std::string m_user;
		std::string m_version;
		std::string m_mime_type;
		std::string m_host;
		std::string m_request_method;
		std::string m_map_name;
		std::string m_encoding;

	private:
		static std::string	DEFAULT_HOST;
		static std::string	DEFAULT_METHOD;
	};

}

#endif //__AUGE_WMTS_REQUEST_H__
