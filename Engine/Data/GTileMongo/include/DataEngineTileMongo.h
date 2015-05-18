#ifndef __AUGE_DATA_ENGINE_MONGO_H__
#define __AUGE_DATA_ENGINE_MONGO_H__

#include <mongoc.h>

#include "AugeData.h"
#include <string>

namespace auge
{
	class DataEngineTileMongo : public DataEngine
	{
	public:
		DataEngineTileMongo();
		virtual ~DataEngineTileMongo();
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

#endif //__AUGE_DATA_ENGINE_MONGO_H__
