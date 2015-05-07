#ifndef __AUGE_ENUM_WORKSPACE_IMPL_H__
#define __AUGE_ENUM_WORKSPACE_IMPL_H__

#include "AugeData.h"
#include <string>

namespace auge
{
	class EnumWorkspaceImpl : public EnumWorkspace
	{
	public:
		EnumWorkspaceImpl();
		virtual ~EnumWorkspaceImpl();
	public:
		virtual void			Reset();
		virtual	Workspace*		Next();
		virtual void			Release();

	public:
		void	Add(Workspace* pWorkspace);
	private:
		std::vector<Workspace*> m_workspaces;
		std::vector<Workspace*>::iterator m_iter;
	};
}

#endif //__AUGE_ENUM_WORKSPACE_IMPL_H__
