#include "EnumWorkspaceImpl.h"

namespace auge
{
	EnumWorkspaceImpl::EnumWorkspaceImpl()
	{
		m_iter = m_workspaces.begin();
	}

	EnumWorkspaceImpl::~EnumWorkspaceImpl()
	{
		std::vector<Workspace*>::iterator iter;
		for(iter=m_workspaces.begin(); iter!=m_workspaces.end(); iter++)
		{
			(*iter)->Release();
		}
		m_workspaces.clear();
	}

	void EnumWorkspaceImpl::Reset()
	{
		m_iter = m_workspaces.begin();
	}

	Workspace* EnumWorkspaceImpl::Next()
	{
		if(m_iter==m_workspaces.end())
		{
			return NULL;
		}
		return *(m_iter++);
	}

	void EnumWorkspaceImpl::Release()
	{
		delete this;
	}

	void EnumWorkspaceImpl::Add(Workspace* pWorkspace)
	{
		m_workspaces.push_back(pWorkspace);
	}
}