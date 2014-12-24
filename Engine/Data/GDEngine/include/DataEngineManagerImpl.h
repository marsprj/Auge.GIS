#ifndef __AUGE_DATA_ENGINE_IMPL_H__
#define __AUGE_DATA_ENGINE_IMPL_H__

#include "AugeData.h"
#include "AugeXML.h"
#include <vector>

namespace auge
{
	class DataEngineManagerImpl : public DataEngineManager
	{
	public:
		DataEngineManagerImpl();
		virtual ~DataEngineManagerImpl();

	public:
		virtual g_uint		GetCount();
		virtual DataEngine*	GetEngine(g_uint i);
		virtual DataEngine*	GetEngine(const char* id);
		virtual DataEngine*	Register(const char* path);
		virtual RESULTCODE	Unregister(const char* id);

		virtual	RESULTCODE	Load(const char* config_path=NULL);
		virtual void		Unload();

	private:
		RESULTCODE	Save();
		void		MakeConfigPath();

		DataEngine*	LoadEngine(const char* path);
		DataEngine*	LoadEngine(XNode* pxNode);
		void		MakeLibraryPath(char* libpath, const char* dir, const char* id);

	private:
		std::vector<DataEngine*>	m_engines;
		std::string		m_path;
	};
}

#endif __AUGE_DATA_ENGINE_IMPL_H__
