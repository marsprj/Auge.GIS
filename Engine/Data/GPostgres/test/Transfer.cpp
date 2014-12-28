#include "Transfer.h"
#include "AugeFeature.h"

//CPPUNIT_TEST_SUITE_REGISTRATION(TransferTest);

void TransferTest::setUp() 
{
	printf("setUp\n");

	auge::GLogger* pLogger = auge::augeGetLoggerInstance();
	pLogger->Initialize();

	auge::DataEngine* pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Load();
	pEngine = pEngineManager->GetEngine("Postgres");
	m_pwsPGS = (auge::FeatureWorksapce*)pEngine->CreateWorkspace();
	m_pwsPGS->SetConnectionString("SERVER=127.0.0.1;INSTANCE=5432;DATABASE=testdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	m_pwsPGS->Open();

	pEngine = pEngineManager->GetEngine("Shape");
	m_pwsSHP = (auge::FeatureWorksapce*)pEngine->CreateWorkspace();
	m_pwsSHP->SetConnectionString("DATABASE=D:\\GeoBeans\\data\\world");
	m_pwsSHP->Open();

}

void TransferTest::tearDown()
{
	m_pwsSHP->Close();
	AUGE_SAFE_RELEASE(m_pwsSHP);
	m_pwsPGS->Close();
	AUGE_SAFE_RELEASE(m_pwsPGS);

	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Unload();
	printf("tearDown\n");
}

void TransferTest::Transfer()
{
	//const char* className = "cities";
	const char* className = "rivers";

	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pwsSHP->OpenFeatureClass(className);
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::GFields* pFields = pFeatureClass->GetFields();
	auge::FeatureClass* pNewClass = NULL;

	m_pwsPGS->RemoveFeatureClass(className);
	m_pwsPGS->CreateFeatureClass(className, pFields);
	pNewClass = m_pwsPGS->OpenFeatureClass(className);

	auge::FeatureInsertCommand* pCommand = NULL;
	pCommand = pNewClass->CreateInsertCommand();

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query();
	CPPUNIT_ASSERT(pCursor!=NULL);

	int index = 1;
	auge::Feature	*pFeature = NULL;

	pCommand->Prepare(pFeatureClass);
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d", index++);
		pCommand->Insert(pFeature);

		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCommand);
	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);
}
