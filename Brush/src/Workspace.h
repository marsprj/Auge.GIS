#ifndef __AUGE_WORKSPACE_H__
#define __AUGE_WORKSPACE_H__

#include "Base.h"
#include <libpq-fe.h>

#include <string>
#include <vector>

namespace auge
{
	class FeatureType;

	class AUGE_API Workspace : public Base
	{
		friend class FeatureType;
		friend class MapManager;
		friend class StyleIO;

	public:
		Workspace();
		virtual ~Workspace();

	public:
		int				Open(const char* connstring);
		int				Close();

		std::vector<FeatureType*>& GetFeatureTypes();
		FeatureType*	OpenFeatureType(const char* szName);

	private:
		void	Cleanup();

	private:
		std::string	m_connstring;
		PGconn		*m_pgConnection;

		std::vector<FeatureType*>	m_types;
	};

}//namespace

#endif //__AUGE_WORKSPACE_H__