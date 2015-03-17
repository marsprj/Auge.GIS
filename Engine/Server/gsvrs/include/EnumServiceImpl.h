#ifndef __AUGE_ENUM_SERVICE_IMPL_H__
#define __AUGE_ENUM_SERVICE_IMPL_H__

#include <AugeService.h>
#include <vector>

namespace auge
{
	class EnumServiceImpl : public EnumService
	{
	public:
		EnumServiceImpl();
		virtual ~EnumServiceImpl();
	public:
		virtual void		Reset();
		virtual Service*	Next();

	public:
		virtual	void		Add(Service* pService);
	private:
		std::vector<Service*>	m_services;
		std::vector<Service*>::iterator m_iter;
	};
}

#endif //__AUGE_ENUM_SERVICE_IMPL_H__
