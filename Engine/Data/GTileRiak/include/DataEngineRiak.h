#ifndef __AUGE_DATA_ENGINE_RIAK_H__
#define __AUGE_DATA_ENGINE_RIAK_H__

#include "AugeData.h"
#include <string>

namespace auge
{
	class DataEngineRiak : public DataEngine
	{
	public:
		DataEngineRiak();
		virtual ~DataEngineRiak();
	public:
		virtual const char*			GetID();
		virtual const char*			GetDescription();

		virtual void*				GetHandler();
		virtual void				SetHandler(void* handler);

		virtual const char*			GetLibraryPath();

	public:
		virtual GConnection*		CreateConnection();
		virtual Workspace*			CreateWorkspace();

	private:
		void*		m_handler;
		std::string	m_path;
	};
}

#endif //__AUGE_DATA_ENGINE_RIAK_H__
