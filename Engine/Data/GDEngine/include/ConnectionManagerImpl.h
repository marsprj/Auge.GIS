#ifndef __AUGE_CONNECTION_MANAGER_IMPL_H__
#define __AUGE_CONNECTION_MANAGER_IMPL_H__

#include "AugeData.h"
#include <vector>

namespace auge
{
	class ConnectionManagerImpl : public ConnectionManager
	{
	public:
		ConnectionManagerImpl();
		virtual ~ConnectionManagerImpl();
	public:
		virtual g_int				GetCount();
		virtual Workspace*			GetWorkspace(g_uint i);
		virtual Workspace*			GetWorkspace(const char* name);
		virtual Workspace*			GetWorkspaceById(g_uint id);
		virtual EnumWorkspace*		GetWorkspaces(); 

		virtual Workspace*			NewWorkspace(const char* name);

		virtual RESULTCODE			Register(const char* name, const char* engine, const char* constr);
		virtual RESULTCODE			Unregister(const char* name);
		virtual RESULTCODE			Update(const char* name, const char* engine, const char* constr);
		virtual RESULTCODE			Update(g_uint gid, const char* name, const char* engine, const char* constr);

		virtual bool				Has(const char* name);
		virtual g_int				GetID(const char* name);

		virtual RESULTCODE			Initialize(GConnection* pConnection);
		virtual void				Unload();



	private:
		bool		CreateDataSourceTable();
		Workspace*	NewWorkspace(g_uint gid, const char* name, const char* engine, const char* uri, g_uint version);
		Workspace*	LoadWorkspace(const char* name);
		RESULTCODE	SaveWorkspace(const char* name, const char* engine, const char* uri);

		bool		IsUpdated(Workspace* pWorkspace);
		g_int		GetWorkspaceVersion(g_uint gid);

	private:
		GConnection* m_pConnection;

		std::vector<Workspace*> m_connections;
	};
}

#endif //__AUGE_CONNECTION_MANAGER_IMPL_H__
