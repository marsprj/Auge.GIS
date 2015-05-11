#ifndef __AUGE_DATA_ENGINE_IMPL_H__
#define __AUGE_DATA_ENGINE_IMPL_H__

#include "AugeFeature.h"
#include <string>

namespace auge
{
	class DataEngineMy : public DataEngine
	{
	public:
		DataEngineMy();
		virtual ~DataEngineMy();
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

#endif //__AUGE_DATA_ENGINE_IMPL_H__
