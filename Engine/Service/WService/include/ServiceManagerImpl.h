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
		virtual Service*		GetService(const char* szName);
		virtual EnumService*	GetServices();

		virtual RESULTCODE		Register(const char* szName);
		virtual RESULTCODE		Unregister(const char* szName);

		virtual RESULTCODE		RegisterMap(g_uint s_id, g_uint m_id);

		virtual RESULTCODE		Unload();

	public:
		virtual RESULTCODE		Initialize(GConnection* pConnection);

	private:
		void		Cleanup();
		bool		CreateServiceTable();

		Service*	LoadService(const char* szName);
	private:
		GConnection* m_pConnection;

		std::vector<Service*>	m_services;
	};
}

#endif //__AUGE_SERVICE_MANAGER_IMPL_H__
