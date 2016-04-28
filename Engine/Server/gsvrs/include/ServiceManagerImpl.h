#ifndef __AUGE_SERVICE_MANAGER_IMPL_H__
#define __AUGE_SERVICE_MANAGER_IMPL_H__

#include "AugeService.h"
#include <vector>

namespace auge
{
	class GConnection;
	class ServiceManagerImpl : public ServiceManager
	{
	public:
		ServiceManagerImpl();
		virtual ~ServiceManagerImpl();
	public:
		virtual g_uint			GetCount();
		virtual Service*		GetService(g_uint user_id, const char* szName);
		virtual Service*		LoadService(g_uint user_id, const char* szName);
		virtual EnumService*	GetServices(g_uint user_id);
		virtual g_int			GetServiceID(g_uint user_id, const char* szName);
		virtual bool			Has(g_uint user_id, const char* szName);
		
		virtual RESULTCODE		Register(g_uint user_id, const char* szName, const char* szURI);
		virtual RESULTCODE		Register(g_uint user_id, const char* szName, const char* mapName, const char* szURI);
		virtual RESULTCODE		Unregister(g_uint user_id, const char* szName);

		virtual RESULTCODE		Start(g_uint user_id, const char* szName);
		virtual RESULTCODE		Stop(g_uint user_id, const char* szName);

		virtual RESULTCODE		RegisterMap(g_uint s_id, g_uint m_id);

		virtual RESULTCODE		Remove(g_uint user_id, const char* szName);
		virtual RESULTCODE		Unload();

	public:
		virtual RESULTCODE		Initialize(GConnection* pConnection);

	private:
		void		Cleanup();
		bool		CreateServiceTable();

		
	private:
		GConnection* m_pConnection;

		std::vector<Service*>	m_services;
	};
}

#endif //__AUGE_SERVICE_MANAGER_IMPL_H__
