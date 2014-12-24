#ifndef __AUGE_SERVICE_IMPL_H__
#define __AUGE_SERVICE_IMPL_H__

#include "AugeService.h"
#include "AugeCarto.h"
#include <string>

namespace auge
{
	class ServiceImpl : public Service
	{
	public:
		ServiceImpl();
		virtual ~ServiceImpl();
	public:
		virtual const char*		GetName();
		virtual WebResponse*	Execute(WebRequest* pRequest);

		virtual	RESULTCODE		Start();
		virtual RESULTCODE		Shutdown();

		virtual void			Release();

	public:
		Map*	GetMap();
		void	SetMap(Map* pMap);
		void	SetMapName(const char* name);
		

	private:
		bool	Support(const char* engine);

	private:
		std::string m_name;
		
		std::string m_engines;
		WebContext* m_pContext;

		std::string m_map_name;
		Map*		m_pMap;
	};
}

#endif //__AUGE_SERVICE_IMPL_H__
