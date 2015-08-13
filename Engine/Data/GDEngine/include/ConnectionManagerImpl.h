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
		virtual EnumWorkspace*		GetWorkspaces(augeWorkspaceType type);

		virtual Workspace*			NewWorkspace(const char* name);

		virtual RESULTCODE			Register(const char* name, const char* engine, const char* constr, augeWorkspaceType type);
		virtual RESULTCODE			Unregister(const char* name);
		virtual RESULTCODE			Update(const char* name, const char* engine, const char* constr);
		virtual RESULTCODE			Update(g_uint gid, const char* name, const char* engine, const char* constr);

		virtual bool				Has(const char* name);
		virtual g_int				GetID(const char* name);

		//------------------------------------------------------------------------
		// DataSource User Methods Begin
		//------------------------------------------------------------------------
		virtual g_int				GetCount(g_uint user_id);
		virtual Workspace*			GetWorkspace(g_uint user_id, const char* name);
		/*virtual Workspace*			GetWorkspaceById(g_uint user_id, g_uint id);*/
		virtual EnumWorkspace*		GetWorkspaces(g_uint user_id);
		virtual EnumWorkspace*		GetWorkspaces(g_uint user_id, augeWorkspaceType type);

		virtual Workspace*			NewWorkspace(g_uint user_id, const char* name);

		virtual RESULTCODE			Register(g_uint user_id, const char* name, const char* engine, const char* constr, augeWorkspaceType type);
		virtual RESULTCODE			Unregister(g_uint user_id, const char* name);
		//virtual RESULTCODE			Update(g_uint user_id, const char* name, const char* engine, const char* constr);
		//virtual RESULTCODE			Update(g_uint user_id, g_uint gid, const char* name, const char* engine, const char* constr);

		virtual bool				Has(g_uint user_id, const char* name);
		virtual g_int				GetID(g_uint user_id, const char* name);
		//------------------------------------------------------------------------
		// DataSource User Methods End
		//------------------------------------------------------------------------

		virtual RESULTCODE			Initialize(GConnection* pConnection);
		virtual void				Unload();



	private:
		bool		CreateDataSourceTable();
		Workspace*	NewWorkspace(g_uint gid, const char* name, const char* engine, const char* uri, g_uint version);
		Workspace*	LoadWorkspace(const char* name);
		RESULTCODE	SaveWorkspace(const char* name, const char* engine, const char* uri, augeWorkspaceType type);

		Workspace*	NewWorkspace(g_uint user_id, g_uint gid, const char* name, const char* engine, const char* uri, g_uint version);
		Workspace*	LoadWorkspace(g_uint user_id, const char* name);
		RESULTCODE	SaveWorkspace(g_uint user_id, const char* name, const char* engine, const char* uri);
		RESULTCODE	SaveWorkspace(g_uint user_id, const char* name, const char* engine, const char* uri, augeWorkspaceType type);

		bool		IsUpdated(Workspace* pWorkspace);
		g_int		GetWorkspaceVersion(g_uint gid);

	private:
		GConnection* m_pConnection;

		std::vector<Workspace*> m_connections;
	};
}

#endif //__AUGE_CONNECTION_MANAGER_IMPL_H__
