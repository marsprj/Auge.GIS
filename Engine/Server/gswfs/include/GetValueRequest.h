#ifndef __AUGE_WFS_GETVALUE_REQUEST_H__
#define __AUGE_WFS_GETVALUE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include "AugeFilter.h"
#include <string>
#include <vector>
#include "WFeatureRequest.h"

namespace auge
{
	class GetValueRequest : public WFeatureRequest
	{
	public:
		GetValueRequest();
		virtual ~GetValueRequest();
	public:
		virtual const char*		GetEngine();
		virtual const char*		GetUser();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetServiceName();
		virtual const char*		GetMimeType();
		virtual const char*		GetTypeName();
		virtual const char*		GetEncoding();
		
		virtual const char*		GetField();
		virtual augeOrderType	GetOrder();

	public:
		bool		Create(rude::CGI& cgi);
		void		SetVersion(const char* value);
		void		SetUser(const char* user);
		void		SetServiceName(const char* serviceName);
		void		SetTypeName(const char* typeName);
		void		SetEncoding(const char* encoding);

		void		SetField(const char* field);
		void		SetOrder(const char* order);

		const char* GetType();
		void		SetType(const char* type);

	private:
		std::string m_user;
		std::string m_version;
		std::string m_mime_type;
		std::string m_type_name;
		std::string m_service_name;
		std::string m_field;
		std::string m_encoding;
		std::string m_type;
		augeOrderType m_order;

		std::string m_name;

	};
}

#endif //__AUGE_WFS_GETVALUE_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=wfs&version=1.0.0&request=GetValue&typeName=cities&field=country&type=unique
user=user1&servicename=world&service=wfs&version=1.0.0&request=GetValue&typeName=cities&field=country
http://127.0.0.1:8088/ows/user1/world/ims?service=wfs&version=1.0.0&request=GetValue&typeName=cities&field=country&type=unique
http://192.168.111.160:8088/ows/user1/world/ims?service=wfs&version=1.0.0&request=GetValue&typeName=cities&field=country&type=unique

http://127.0.0.1:8088/ows/user1/mgr?service=wfs&version=1.0.0&request=GetValue&typeName=cities&field=country&type=unique&sourceName=db1

http://192.168.111.160:8088/ows/user1/world/ims?service=wfs&version=1.0.0&request=GetValue&typeName=cities&field=country&type=minmax

************************************************************************/