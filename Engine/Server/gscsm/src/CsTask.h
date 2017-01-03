#ifndef __CSTASK_H__
#define __CSTASK_H__

#include "AugeCore.h"

namespace auge
{
	class CsTask : public GObject
	{
	public:
		CsTask();
		virtual ~CsTask();

	public:
		g_int		GetMapID();
		g_int		GetOwner();
		const char*	GetName();
		const char*	GetDescription();

	public:
		bool	Create(g_int tid, const char* name, const char* description, g_int mapID, g_int owner);

	private:
		g_int	m_mid;
		g_int	m_owner;
		std::string m_name;
		std::string	m_description;
	};
}

#endif //__CSTASK_H__
