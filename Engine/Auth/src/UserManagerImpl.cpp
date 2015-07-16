#include "UserManagerImpl.h"
#include "EnumRoleImpl.h"
#include "EnumUserImpl.h"
#include "RoleImpl.h"
#include "UserImpl.h"
#include "AugeData.h"

namespace auge
{
	#define AUGE_USER_TABLE	"g_user"
	#define AUGE_ROLE_TABLE	"g_role"
	
	UserManager* augeGetUserManagerInstance()
	{
		static UserManagerImpl g_userManager;
		return &g_userManager;
	}

	UserManagerImpl::UserManagerImpl()
	{
		m_pConnection = NULL;
		m_pUser = new UserImpl();
	}

	UserManagerImpl::~UserManagerImpl()
	{
		m_pUser->Release();
	}

	User* UserManagerImpl::CreateUser(const char* name, const char* alias, const char* password, const char* email, const char* role)
	{
		if(name==NULL)
		{
			return NULL;
		}

		User* pUser = GetUser(name);
		if(pUser!=NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "User [%s] already exits", name);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return NULL;
		}

		Role* pRole = GetRole(role);
		if(pRole==NULL)
		{
			return NULL;
		}

		if(name==NULL||alias==NULL||password==NULL||email==NULL)
		{
			pRole->Release();
			return NULL;
		}

		char sql[AUGE_SQL_MAX];
		g_sprintf(sql, "insert into g_user (name,alias,passwd,email,role) values('%s','%s','%s','%s',%d) returning gid", name, alias, password, email, pRole->GetID());
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			pRole->Release();
			return NULL;
		}

		if(!pResult->GetCount())
		{
			pResult->Release();
			pRole->Release();
			return NULL;
		}

		int gid = pResult->GetInt(0,0);
		pResult->Release();

		UserImpl* pUserImpl = new UserImpl();
		pUserImpl->Create(gid, name, alias, password, email);
		pUserImpl->SetRole(pRole);

		return pUserImpl;
	}

	RESULTCODE UserManagerImpl::RemoveUser(const char* name)
	{
		if(name==NULL)
		{
			return AG_FAILURE;
		}
		char sql[AUGE_SQL_MAX];
		g_sprintf(sql, "delete from g_user where name='%s'",name);
		return m_pConnection->ExecuteSQL(sql);
	}

	//User* UserManagerImpl::GetUser(const char* name)
	//{
	//	if(name==NULL)
	//	{
	//		return NULL;
	//	}
	//	m_pUser->SetName(name);
	//	return m_pUser;
	//}

	User* UserManagerImpl::GetUser(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}
		char sql[AUGE_SQL_MAX];
		g_sprintf(sql, "select u.gid,u.name,u.alias,u.passwd,u.email,u.role, r.name,r.alias from g_user u, g_role r where u.role=r.gid and u.name='%s'",name);
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		int count = pResult->GetCount();
		if(!count)
		{
			return NULL;
		}

		int u_gid = pResult->GetInt(0,0);
		const char* u_name = pResult->GetString(0,1);
		const char* u_alias = pResult->GetString(0,2);
		const char* u_passwd = pResult->GetString(0,3);
		const char* u_email = pResult->GetString(0,4);

		int r_gid = pResult->GetInt(0,5);
		const char* r_name = pResult->GetString(0,7);
		const char* r_alias = pResult->GetString(0,7);

		RoleImpl* pRole = new RoleImpl();
		pRole->Create(r_gid, r_name,r_alias);
		UserImpl* pUser = new UserImpl();
		pUser->Create(u_gid, u_name, u_alias, u_passwd, u_email);
		pUser->SetRole(pRole);

		pResult->Release();

		return pUser;
	}

	EnumUser* UserManagerImpl::GetUsers()
	{
		const char* sql ="select u.gid,u.name,u.alias,u.passwd,u.email,u.role, r.name,r.alias from g_user u, g_role r where u.role=r.gid";
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		EnumUserImpl* pUsers = new EnumUserImpl();
		int count = pResult->GetCount();
		for(int i=0; i<count; i++)
		{
			int u_gid = pResult->GetInt(i,0);
			const char* u_name = pResult->GetString(i,1);
			const char* u_alias = pResult->GetString(i,2);
			const char* u_passwd = pResult->GetString(i,3);
			const char* u_email = pResult->GetString(i,4);

			int r_gid = pResult->GetInt(i,5);
			const char* r_name = pResult->GetString(i,6);
			const char* r_alias = pResult->GetString(i,7);

			RoleImpl* pRole = new RoleImpl();
			pRole->Create(r_gid, r_name,r_alias);
			UserImpl* pUser = new UserImpl();
			pUser->Create(u_gid, u_name, u_alias, u_passwd, u_email);
			pUser->SetRole(pRole);

			pUsers->Add(pUser);
		}
		pResult->Release();

		return pUsers;
	}

	Role* UserManagerImpl::GetRole(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}
		char sql[AUGE_SQL_MAX];
		g_sprintf(sql, "select gid, alias from g_role where name='%s'", name);
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		int count = pResult->GetCount();
		if(!count)
		{
			pResult->Release();
			return NULL;
		}

		int gid = pResult->GetInt(0,0);
		const char* alias = pResult->GetString(0,1);
		RoleImpl* pRole = new RoleImpl();
		pRole->Create(gid, name, alias);

		pResult->Release();

		return pRole;
	}

	EnumRole* UserManagerImpl::GetRoles()
	{
		const char* sql = "select gid, name, alias from g_role";
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		EnumRoleImpl* pRoles = new EnumRoleImpl();
		int count = pResult->GetCount();
		for(int i=0; i<count; i++)
		{
			int gid = pResult->GetInt(i,0);
			const char* name = pResult->GetString(i,1);
			const char* alias = pResult->GetString(i,2);
			RoleImpl* pRole = new RoleImpl();
			pRole->Create(gid, name, alias);
			pRoles->Add(pRole);
		}
		pResult->Release();

		return pRoles;
	}

	RESULTCODE UserManagerImpl::Initialize(GConnection* pConnection)
	{
		if(pConnection==NULL)
		{
			return AG_FAILURE;
		}

		RESULTCODE rc = AG_SUCCESS;
		m_pConnection = pConnection;
		if(!m_pConnection->HasTable(AUGE_ROLE_TABLE))
		{
			rc = CreateRoleTable();
			if(rc!=AG_SUCCESS)
			{
				return AG_FAILURE;
			}
			else
			{
				InitRoleTable();
			}
		}

		if(!m_pConnection->HasTable(AUGE_USER_TABLE))
		{
			rc = CreateUserTable();
			if(rc!=AG_SUCCESS)
			{
				return AG_FAILURE;
			}
			else
			{
				InitUserTable();
			}
		}

		return AG_SUCCESS;
	}

	RESULTCODE UserManagerImpl::CreateRoleTable()
	{
		const char* sql = "CREATE TABLE g_role(gid serial NOT NULL, name character varying(16), alias character varying(16), CONSTRAINT role_uk UNIQUE (gid))";
		return m_pConnection->ExecuteSQL(sql);
	}

	RESULTCODE UserManagerImpl::InitRoleTable()
	{
		const char* sql = "insert into g_role (name,alias) values('admin','����Ա')";
		m_pConnection->ExecuteSQL(sql);
		sql = "insert into g_role (name,alias) values('normal','��ͨ')";
		m_pConnection->ExecuteSQL(sql);
		sql = "insert into g_role (name,alias) values('public','����')";
		m_pConnection->ExecuteSQL(sql);

		return AG_SUCCESS;
	}

	RESULTCODE UserManagerImpl::CreateUserTable()
	{
		const char* sql = "CREATE TABLE g_user( gid serial NOT NULL, name character varying(32) NOT NULL, alias character varying(32) NOT NULL, passwd character varying(16), email character varying, role integer, CONSTRAINT user_pk PRIMARY KEY (gid), CONSTRAINT user_fk FOREIGN KEY (role) REFERENCES g_role (gid) MATCH SIMPLE ON UPDATE NO ACTION ON DELETE NO ACTION)";
		return m_pConnection->ExecuteSQL(sql);
	}

	RESULTCODE UserManagerImpl::InitUserTable()
	{
		const char* sql = "insert into g_user (name,alias,passwd,email,role) values('admin','����Ա', 'qwer1234','admin@mapcloud.org',1)";
		m_pConnection->ExecuteSQL(sql);
		sql = "insert into g_user (name,alias,passwd,email,role) values('user1','�û�1',  'qwer1234','user1@mapcloud.org',2)";
		return m_pConnection->ExecuteSQL(sql);
	}

}