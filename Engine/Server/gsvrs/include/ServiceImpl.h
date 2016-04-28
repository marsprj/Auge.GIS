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
		virtual const char*		GetURI();
		

		virtual WebResponse*	Execute(WebRequest* pRequest);

		virtual Map*			GetMap();
		virtual void			SetMap(Map* pMap);
		virtual const char*		GetMapName();
		virtual WebContext*		GetWebContext();

		virtual	RESULTCODE		Start();
		virtual RESULTCODE		Shutdown();
		virtual	RESULTCODE		Pause();
		virtual RESULTCODE		Resume();
		virtual bool			IsStarted();

		virtual void			Release();

	public:
		
		void	SetMapName(const char* name);
		void	SetName(const char* name);
		void	SetURI(const char* uri);
		void	SetState(g_uint state);

	private:
		bool	Support(const char* engine);

	private:
		std::string m_name;
		std::string m_uri;
		g_uint		m_state;
		
		std::string m_engines;
		WebContext* m_pContext;

		std::string m_map_name;
		Map*		m_pMap;
	};
}

#endif //__AUGE_SERVICE_IMPL_H__
