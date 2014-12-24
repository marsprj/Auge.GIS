#ifndef __AUGE_WEB_ENGINE_MANAGER_IMPL_H__
#define __AUGE_WEB_ENGINE_MANAGER_IMPL_H__

#include "AugeService.h"
#include "AugeXML.h"
#include <vector>
#include <string>

namespace auge
{
	class WebEngineManagerImpl : public WebEngineManager
	{
	public:
		WebEngineManagerImpl();
		virtual ~WebEngineManagerImpl();
	public:


		virtual g_uint			GetCount();
		virtual WebEngine*		GetEngine(const char* type);

		virtual WebEngine*		Register(const char* path);
		virtual RESULTCODE		Unregister(const char* id);

		virtual	RESULTCODE		Load(const char* config_path=NULL);
		virtual void			Unload();

	private:
		WebEngine*	Find(const char* id);

		RESULTCODE	Save();

		WebEngine*	LoadEngine(const char* path);
		WebEngine*	LoadEngine(XNode* pxNode);
		RESULTCODE	UnloadEngine(WebEngine* pEngine);

		void		MakeConfigPath();
		void		MakeLibraryPath(char* libpath, const char* dir, const char* id);

	private:
		std::vector<WebEngine*>	m_engines;
		std::string m_path;
	};
}

#endif //__AUGE_WEB_ENGINE_MANAGER_IMPL_H__
