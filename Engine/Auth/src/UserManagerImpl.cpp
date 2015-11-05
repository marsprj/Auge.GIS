#include "UserManagerImpl.h"
#include "EnumRoleImpl.h"
#include "EnumUserImpl.h"
#include "RoleImpl.h"
#include "UserImpl.h"
#include "AugeData.h"
#include "AugeType.h"

namespace auge
{
	#define AUGE_USER_TABLE	"g_user"
	#define AUGE_ROLE_TABLE	"g_role"
	#define AUGE_USER_LOGIN_TABLE "g_user_login"
	
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
		pUserImpl->Create(gid, name, alias, password, email,0);
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
		g_sprintf(sql, "select u.gid,u.name,u.alias,u.passwd,u.email,u.role, u.status,r.name,r.alias from g_user u, g_role r where u.role=r.gid and u.name='%s'",name);
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
		g_int logined = pResult->GetInt(0, 5);

		int r_gid = pResult->GetInt(0,6);
		const char* r_name = pResult->GetString(0,7);
		const char* r_alias = pResult->GetString(0,8);

		RoleImpl* pRole = new RoleImpl();
		pRole->Create(r_gid, r_name,r_alias);
		UserImpl* pUser = new UserImpl();
		pUser->Create(u_gid, u_name, u_alias, u_passwd, u_email, logined);
		pUser->SetRole(pRole);

		pResult->Release();

		return pUser;
	}

	EnumUser* UserManagerImpl::GetUsers(g_uint count/*=0*/, g_uint offset/*=0*/)
	{
		//const char* sql ="select u.gid,u.name,u.alias,u.passwd,u.email,u.role, u.status, r.name,r.alias from g_user u, g_role r where u.role=r.gid";
		char sql[AUGE_SQL_MAX];
		if(count==0)
		{
			//const char* sql ="select u.gid,u.name,u.alias,u.passwd,u.email,u.role,u.status, r.name,r.alias from g_user u, g_role r where u.role=r.gid and status=1";
			const char* format ="select u.gid,u.name,u.alias,u.passwd,u.email,u.role,u.status, r.name,r.alias from g_user u, g_role r where u.role=r.gid offset %d";
			g_snprintf(sql, AUGE_SQL_MAX, format, offset);
		}
		else
		{
			const char* format ="select u.gid,u.name,u.alias,u.passwd,u.email,u.role,u.status, r.name,r.alias from g_user u, g_role r where u.role=r.gid limit %d offset %d";
			g_snprintf(sql, AUGE_SQL_MAX, format, count, offset);
		}
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		EnumUserImpl* pUsers = new EnumUserImpl();
		int user_count = pResult->GetCount();
		for(int i=0; i<user_count; i++)
		{
			int u_gid = pResult->GetInt(i,0);
			const char* u_name = pResult->GetString(i,1);
			const char* u_alias = pResult->GetString(i,2);
			const char* u_passwd = pResult->GetString(i,3);
			const char* u_email = pResult->GetString(i,4);
			int u_logined = pResult->GetInt(i,5);

			int r_gid = pResult->GetInt(i,6);
			const char* r_name = pResult->GetString(i,7);
			const char* r_alias = pResult->GetString(i,8);

			RoleImpl* pRole = new RoleImpl();
			pRole->Create(r_gid, r_name,r_alias);
			UserImpl* pUser = new UserImpl();
			pUser->Create(u_gid, u_name, u_alias, u_passwd, u_email, u_logined);
			pUser->SetRole(pRole);

			pUsers->Add(pUser);
		}
		pResult->Release();

		return pUsers;
	}

	g_uint UserManagerImpl::GetUserCount()
	{
		const char* sql = "select count(*) from g_user";
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}
		g_uint count = pResult->GetInt(0, 0);
		pResult->Release();

		return count;
	}

	EnumUser* UserManagerImpl::GetLoginedUsers(g_uint count/*=0*/, g_uint offset/*=0*/)
	{
		char sql[AUGE_SQL_MAX];
		if(count==0)
		{
			//const char* sql ="select u.gid,u.name,u.alias,u.passwd,u.email,u.role,u.status, r.name,r.alias from g_user u, g_role r where u.role=r.gid and status=1";
			const char* format ="select u.gid,u.name,u.alias,u.passwd,u.email,u.role,u.status, r.name,r.alias from g_user u, g_role r where u.role=r.gid and status=1 and ((now() - login_time) <  '1 day 00:30:00'::interval) offset %d";
			g_snprintf(sql, AUGE_SQL_MAX, format, offset);
		}
		else
		{
			const char* format ="select u.gid,u.name,u.alias,u.passwd,u.email,u.role,u.status, r.name,r.alias from g_user u, g_role r where u.role=r.gid and status=1 and ((now() - login_time) <  '1 day 00:30:00'::interval) limit %d offset %d";
			g_snprintf(sql, AUGE_SQL_MAX, format, count, offset);
		}
		

		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		EnumUserImpl* pUsers = new EnumUserImpl();
		int users = pResult->GetCount();
		for(int i=0; i<users; i++)
		{
			int u_gid = pResult->GetInt(i,0);
			const char* u_name = pResult->GetString(i,1);
			const char* u_alias = pResult->GetString(i,2);
			const char* u_passwd = pResult->GetString(i,3);
			const char* u_email = pResult->GetString(i,4);
			int logined = pResult->GetInt(i, 5);

			int r_gid = pResult->GetInt(i,6);
			const char* r_name = pResult->GetString(i,7);
			const char* r_alias = pResult->GetString(i,8);

			RoleImpl* pRole = new RoleImpl();
			pRole->Create(r_gid, r_name,r_alias);
			UserImpl* pUser = new UserImpl();
			pUser->Create(u_gid, u_name, u_alias, u_passwd, u_email, logined);
			pUser->SetRole(pRole);

			pUsers->Add(pUser);
		}
		pResult->Release();

		return pUsers;
	}

	g_uint UserManagerImpl::GetLoginedUserCount()
	{
		//const char* sql = "select count(*) from g_user where status=1";
		const char* sql = "select count(*) from g_user where status=1 and ((now() - login_time) <  '1 day 00:30:00'::interval)";
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}
		g_uint count = pResult->GetInt(0, 0);
		pResult->Release();

		return count;
	}

	RESULTCODE UserManagerImpl::Login()
	{
		const char* sql = "update g_user set status=1,login_time=now()"; 
		return m_pConnection->ExecuteSQL(sql);
	}

	RESULTCODE UserManagerImpl::Login(const char* name, const char* passwd)
	{
		if(name==NULL||passwd==NULL)
		{
			return AG_FAILURE;
		}

		char sql[AUGE_SQL_MAX];
		g_sprintf(sql, "select count(*) from g_user where name='%s' and passwd='%s'", name, passwd);
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return AG_FAILURE;
		}
		g_uint count = pResult->GetInt(0, 0);
		pResult->Release();
		if(count==0)
		{
			return AG_FAILURE;
		}

		g_sprintf(sql, "update g_user set status=1,login_time=now() where name='%s'", name);
		return m_pConnection->ExecuteSQL(sql);
	}

	RESULTCODE UserManagerImpl::Login(const char* name, const char* passwd, const char* remote_address)
	{
		if(name==NULL||passwd==NULL)
		{
			return AG_FAILURE;
		}

		char sql[AUGE_SQL_MAX];
		g_sprintf(sql, "select count(*) from g_user where name='%s' and passwd='%s'", name, passwd);
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return AG_FAILURE;
		}
		g_uint count = pResult->GetInt(0, 0);
		pResult->Release();
		if(count==0)
		{
			return AG_FAILURE;
		}

		g_sprintf(sql, "update g_user set status=1,login_time=now() where name='%s'", name);
		RESULTCODE rc = m_pConnection->ExecuteSQL(sql);
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}

		g_sprintf(sql, "insert into g_user_login (user_name, login_time, remote_address) values('%s',now(),'%s')", name, remote_address);
		return m_pConnection->ExecuteSQL(sql);;
	}

	bool UserManagerImpl::IsLogined(const char* name)
	{
		if(name==NULL)
		{
			return false;
		}

		User* pUser = GetUser(name);
		if(pUser==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "User [%s] does not exist.", name);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return false;
		}
		bool logined = pUser->IsLogined();
		pUser->Release();
		return logined;
	}

	RESULTCODE UserManagerImpl::Logout(const char* name)
	{
		if(name==NULL)
		{
			return AG_FAILURE;
		}

		char sql[AUGE_SQL_MAX];
		// Set status to login
		g_sprintf(sql, "update g_user set status=0 where name='%s'", name);
		return m_pConnection->ExecuteSQL(sql);
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

		if(!m_pConnection->HasTable(AUGE_USER_LOGIN_TABLE))
		{
			rc = CeateLoginTable();
			if(rc!=AG_SUCCESS)
			{
				return AG_FAILURE;
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
		const char* sql = "insert into g_role (name,alias) values('admin','管理员')";
		m_pConnection->ExecuteSQL(sql);
		sql = "insert into g_role (name,alias) values('normal','普通')";
		m_pConnection->ExecuteSQL(sql);
		sql = "insert into g_role (name,alias) values('public','公共')";
		m_pConnection->ExecuteSQL(sql);

		return AG_SUCCESS;
	}

	RESULTCODE UserManagerImpl::CreateUserTable()
	{
		const char* sql = "CREATE TABLE g_user( gid serial NOT NULL, name character varying(32) NOT NULL, alias character varying(32) NOT NULL, passwd character varying(16), email character varying, role integer, status integer DEFAULT 0, CONSTRAINT user_pk PRIMARY KEY (gid), CONSTRAINT user_fk FOREIGN KEY (role) REFERENCES g_role (gid) MATCH SIMPLE ON UPDATE NO ACTION ON DELETE NO ACTION)";
		return m_pConnection->ExecuteSQL(sql);
	}

	RESULTCODE UserManagerImpl::CeateLoginTable()
	{
		const char* sql = "CREATE TABLE g_user_login (gid serial NOT NULL,  user_name character varying(32), login_time time without time zone, remote_address character varying(128))";
		return m_pConnection->ExecuteSQL(sql);
	}

	RESULTCODE UserManagerImpl::InitUserTable()
	{
		const char* sql = "insert into g_user (name,alias,passwd,email,role) values('admin','管理员', 'qwer1234','admin@mapcloud.org',1)";
		m_pConnection->ExecuteSQL(sql);
		sql = "insert into g_user (name,alias,passwd,email,role) values('user1','用户1',  'qwer1234','user1@mapcloud.org',2)";
		return m_pConnection->ExecuteSQL(sql);
	}

}