#ifndef __AUGE_IMS_GET_SYMBOL_REQUEST_H__
#define __AUGE_IMS_GET_SYMBOL_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GetSymbolRequest : public WebRequest
	{
	public:
		GetSymbolRequest();
		virtual ~GetSymbolRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual const char*		GetName();

	public:
		const char*				GetType();

	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* value);
		void		SetName(const char* name);
		void		SetType(const char* type);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_encoding;

		std::string m_type;
		std::string m_name;
	};
}

#endif //__AUGE_IMS_GET_SYMBOL_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=ims&version=1.0.0&request=GetSymbol&type=marker&name=star
http://127.0.0.1:8088/ows/user1/mgr?service=ims&version=1.0.0&request=GetSymbol&type=marker&name=star

************************************************************************/