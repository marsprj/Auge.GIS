#include "AuthTest.h"
#include "AugeCore.h"
#include "AugeXML.h"
#include "AugeUser.h"

CPPUNIT_TEST_SUITE_REGISTRATION(AuthTest);

void AuthTest::setUp() 
{
	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Load();
	pEngine = pEngineManager->GetEngine("Postgres");
	m_pConnection = pEngine->CreateConnection();
	//m_pConnection->SetConnectionString("SERVER=127.0.0.1;INSTANCE=5432;DATABASE=auge;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	m_pConnection->SetConnectionString("SERVER=192.168.111.160;INSTANCE=5432;DATABASE=auge;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	m_pConnection->Open();
	
	auge::UserManager* pUserManager = auge::augeGetUserManagerInstance();
	pUserManager->Initialize(m_pConnection);

	printf("setUp\n");
}

void AuthTest::tearDown()
{
	printf("tearDown\n");
}

void AuthTest::CreateUser()
{
	auge::UserManager* pUserManager = auge::augeGetUserManagerInstance();
}

void AuthTest::CreateUsers()
{
	char name[AUGE_NAME_MAX];	
	auge::UserManager* pUserManager = auge::augeGetUserManagerInstance();
	for(g_uint i=0; i<100000; i++)
	{
		g_sprintf(name, "user%d", i);
		pUserManager->CreateUser(name, "", "qwer1234", "user@mapcloud.org", "normal");
		printf("\r%s", name);
	}
}

void AuthTest::Login()
{
	RESULTCODE rc = AG_FAILURE;
	auge::UserManager* pUserManager = auge::augeGetUserManagerInstance();
	rc = pUserManager->Login("user1", "qwer1234");
	CPPUNIT_ASSERT(rc==AG_SUCCESS);
}

void AuthTest::Logout()
{
	RESULTCODE rc = AG_FAILURE;
	auge::UserManager* pUserManager = auge::augeGetUserManagerInstance();
	rc = pUserManager->Logout("user1");
	CPPUNIT_ASSERT(rc==AG_SUCCESS);
}

