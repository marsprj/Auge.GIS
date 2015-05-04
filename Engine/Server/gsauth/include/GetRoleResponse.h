#ifndef __AUGE_IMS_GET_ROLE_RESPONSE_H__
#define __AUGE_IMS_GET_ROLE_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class Role;
	class EnumRole;

	class GetRoleResponse : public WebResponse
	{
	public:
		GetRoleResponse(GetRoleRequest* pRequest);
		virtual ~GetRoleResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	public:
		void		SetRoles(EnumRole* pRoles);
		void		SetRole(Role* pRole);

		RESULTCODE	Write(WebWriter* pWriter, EnumRole* pRoles);
		RESULTCODE	Write(WebWriter* pWriter, Role* pRole);

	private:
		Role* m_pRole;
		EnumRole* m_pRoles;
		GetRoleRequest *m_pRequest;	
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
