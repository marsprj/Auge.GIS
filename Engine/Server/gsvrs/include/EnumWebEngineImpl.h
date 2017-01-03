#ifndef __AUGE_ENUM_WEB_ENGINE_IMPL_H__
#define __AUGE_ENUM_WEB_ENGINE_IMPL_H__

#include <AugeWebEngine.h>
#include <vector>

namespace auge
{
	class EnumWebEngineImpl : public EnumWebEngine
	{
	public:
		EnumWebEngineImpl();
		virtual ~EnumWebEngineImpl();
	public:
		virtual void		Reset();
		virtual WebEngine*	Next();

	public:
		virtual	void		Add(WebEngine* pWebEngine);

		WebEngine*			GetEngine(g_uint i);
		WebEngine*			GetEngine(const char* type);
		WebEngine*			GetEngineById(const char* id);
		
		g_uint				GetCount();
	private:
		std::vector<WebEngine*>	m_engines;
		std::vector<WebEngine*>::iterator m_iter;
	};
}

#endif //__AUGE_ENUM_WEB_ENGINE_IMPL_H__
