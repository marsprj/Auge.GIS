#ifndef __AUGE_WORKSPACE_H__
#define __AUGE_WORKSPACE_H__

#include "Base.h"
#include <libpq-fe.h>

#include <string>
#include <vector>

namespace auge
{
	class FeatureType;

	class AUGE_API Connection : public Base
	{
		friend class FeatureType;
		friend class MapIO;
		friend class StyleIO;

	public:
		Connection();
		virtual ~Connection();

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