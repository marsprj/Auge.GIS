#ifndef __AUGE_WFS_TRANSACTION_REQUEST_H__
#define __AUGE_WFS_TRANSACTION_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class TransactionRequest : public WebRequest
	{
	public:
		TransactionRequest();
		virtual ~TransactionRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetTypeName();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();

	public:
		void		SetVersion(const char* value);
		void		SetTypeName(const char* typeName);
		bool		Create(rude::CGI& cgi);

	private: 
		std::string m_version;
		std::string m_type_name;
		std::string m_full_name;
		std::string m_mime_type;
	};
}

#endif //__AUGE_WFS_TRANSACTION_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=wfs&version=1.0.0&request=Transaction&typeName=world:cities
user=user1&servicename=world&service=wfs&version=1.0.0&request=Transaction&typeName=world:cities
http://127.0.0.1:8088/ows/user1/world/ims?service=wfs&version=1.0.0&request=Transaction&typeName=world:cities

************************************************************************/
