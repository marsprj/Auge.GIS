#ifndef __AUGE_WORKSPACE_SHP_H__
#define __AUGE_WORKSPACE_SHP_H__

#include "AugeFeature.h"
#include <string>
#include <map>

#include "ConnectionPgs.h"

namespace auge
{
	class WorkspacePgs : public FeatureWorksapce
	{
		friend class FeatureClassPgs;

	public:
		WorkspacePgs();
		virtual ~WorkspacePgs();
	public:
		virtual const char*		GetName();
		virtual void			SetName(const char* name);
		virtual	DataEngine*		GetEngine();
	public:
		/* DATABASE=d:\\data\\world */
		virtual	RESULTCODE		SetConnectionString(const char* conn_string);

		virtual RESULTCODE		Open();
		virtual void			Close();
		virtual bool			IsOpen();

		virtual	FeatureClass*	OpenFeatureClass(const char* name);
		
	private:
		ConnectionPgs	m_pgConnection;
		std::string		m_name;
	};
}

#endif //__AUGE_WORKSPACE_SHP_H__
