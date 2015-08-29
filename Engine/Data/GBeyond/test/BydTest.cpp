#include "BydTest.h"
#include "AugeCore.h"
#include "AugeFeature.h"

#include "AugeTile.h"

CPPUNIT_TEST_SUITE_REGISTRATION(BydTest);

void BydTest::setUp() 
{
	const char* path = "SERVER=127.0.0.1;INSTANCE=II;DATABASE=gisdb;USER=gisdb;PASSWORD=qwer1234";
	//const char* path = "SERVER=192.168.111.150;INSTANCE=II;DATABASE=gisdb;USER=gisdb;PASSWORD=qwer1234";
	//const char* path = "SERVER=192.168.111.151;INSTANCE=II;DATABASE=gisdb;USER=gisdb;PASSWORD=qwer1234";

	auge::GLogger	*pLogger = auge::augeGetLoggerInstance();
	pLogger->Initialize();

	RESULTCODE rc = AG_FAILURE;
	auge::DataEngine* pDataEngine = NULL;
	pDataEngine = auge::augeGetDataEngineInstance();
	
	m_pWorkspace = dynamic_cast<auge::FeatureWorkspace*>(pDataEngine->CreateWorkspace());
	m_pWorkspace->SetConnectionString(path);

	rc = m_pWorkspace->Open();
	CPPUNIT_ASSERT(rc==AG_SUCCESS);

	printf("setUp\n");
}

void BydTest::tearDown()
{
	m_pWorkspace->Close();	
	AUGE_SAFE_RELEASE(m_pWorkspace);
	printf("tearDown\n");
}

void BydTest::ReadTest()
{
	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	//pFeatureClass = m_pWorkspace->OpenFeatureClass("rivers");
	//pFeatureClass = m_pWorkspace->OpenFeatureClass("gc_aqi");
	//pFeatureClass = m_pWorkspace->OpenFeatureClass("高等院校");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::GFields* pFields = pFeatureClass->GetFields();

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query();
	CPPUNIT_ASSERT(pCursor!=NULL);

	char str[AUGE_PATH_MAX];

	int counter = 1;
	g_uchar* wkb = NULL;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		//printf("\r%d", counter++);
		//printf("[%d]\n", pFeature->GetFID());
		sprintf(str, "%f", pFeature->GetFloat("cities_id"));
		printf("[%d]:%s\n", pFeature->GetFID(), pFeature->GetString("name"));
		//float val = pFeature->GetFloat("co");
		pGeometry = pFeature->GetGeometry();
		printf("%s\n", pGeometry->AsText());
		//wkb = pGeometry->AsBinary();

		//printf("[Name]:%s\n", pFeature->GetString("name"));
		//printf("[%d]:%s\n", counter++, pFeature->GetString("name"));
		//auge::WKBPoint* pWKBPoint = (auge::WKBPoint*)wkb;
		//printf("[%d]:%f,%f\n", pFeature->GetFID(),pWKBPoint->point.x, pWKBPoint->point.y);

		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);
}

void BydTest::QueryExent()
{
	auge::GEnvelope extent(0,0,180,90);

	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(extent);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);
}

void BydTest::QueryBinaryComparision()
{
	auge::BinaryComparisonFilter* pFilter = NULL;
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();

//	auge::PropertyName* pProp = pFactory->CreatePropertyName("country");
//	auge::GValue* pValue = new auge::GValue("China");
//	auge::Literal* pLiteral = pFactory->CreateLiteral(pValue);

	auge::PropertyName* pProp = pFactory->CreatePropertyName("gid");
	auge::GValue* pValue = new auge::GValue(10);
	auge::Literal* pLiteral = pFactory->CreateLiteral(pValue);

	pFilter = pFactory->CreateBinaryComparisonFilter();
	pFilter->SetExpression1(pProp);
	pFilter->SetExpression2(pLiteral);
	pFilter->SetOperator(auge::augeComOprLessThan);	

	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pFilter);
	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);
}

void BydTest::QueryBetween()
{
	auge::ComparisonFilter* pFilter = NULL;
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();

	//	auge::PropertyName* pProp = pFactory->CreatePropertyName("country");
	//	auge::GValue* pValue = new auge::GValue("China");
	//	auge::Literal* pLiteral = pFactory->CreateLiteral(pValue);

	auge::PropertyName* pProp = pFactory->CreatePropertyName("id");
	auge::GValue* pValue = new auge::GValue(10);
	auge::Literal* pLowerBound = pFactory->CreateLiteral(pValue);
	pValue = new auge::GValue(20);
	auge::Literal* pUpperBound = pFactory->CreateLiteral(pValue);

	pFilter = pFactory->CreateIsBetweenFilter(pProp, pLowerBound, pUpperBound);

	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pFilter);
	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);
}

void BydTest::QueryBetween_2()
{

	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::GFilter* pFilter = NULL;
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();
	auge::FilterReader* reader = pFactory->CreateFilerReader(pFeatureClass->GetFields());

	auge::XParser parser;
	auge::XDocument	*pxDoc = NULL;
	auge::XElement	*pxFilter = NULL;

	const char* path = "E:\\Research\\Auge.GIS\\Engine\\Filter\\ogc\\Between.xml";
	pxDoc = parser.Parse(path);
	pxFilter = pxDoc->GetRootNode();

	pFilter = reader->Read(pxFilter);


	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pFilter);
	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);
}

void BydTest::QueryLike()
{
	auge::ComparisonFilter* pFilter = NULL;
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();

	//	auge::PropertyName* pProp = pFactory->CreatePropertyName("country");
	//	auge::GValue* pValue = new auge::GValue("China");
	//	auge::Literal* pLiteral = pFactory->CreateLiteral(pValue);

	auge::PropertyName* pProp = pFactory->CreatePropertyName("name");
	auge::GValue* pValue = new auge::GValue("C!");
	auge::Literal* pLiteral = pFactory->CreateLiteral(pValue);

	pFilter = pFactory->CreateIsLikeFilter(pProp, pLiteral);

	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("country");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pFilter);
	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);
}

void BydTest::QueryTest()
{
	auge::FilterFactory* pFilterFactory = auge::augeGetFilterFactoryInstance();

	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::GQuery	*pQuery = NULL;
	auge::OrderBy	*pOrderBy = NULL;
	auge::GFilter	*pFilter = NULL;

	auge::GEnvelope extent(0,0,10,10);
	auge::PropertyName	*pProp = pFilterFactory->CreatePropertyName("geom");
	pFilter = pFilterFactory->CreateBBoxFilter(pProp, extent);
	
	pQuery = pFilterFactory->CreateQuery();
	pQuery->AddSubField("gid");
	pQuery->AddSubField("name");
	pQuery->AddSubField("geom");

	pQuery->SetFilter(pFilter);

	pOrderBy = pFilterFactory->CreateOrderBy();
	pOrderBy->AddField("name");
	pOrderBy->SetOrder(auge::augeOrderDesc);
	pQuery->SetOrderBy(pOrderBy);

	auge::FeatureCursor* pCursor = NULL;
	//pCursor = pFeatureClass->Query(pQuery);
	pCursor = pFeatureClass->Query();
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		const char* name = pFeature->GetString("name");
		printf("[%d]:%s\n",pFeature->GetFID(), name);

		pGeometry = pFeature->GetGeometry();
		printf(pGeometry->AsText());

		pFeature->Release();
	}
	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pQuery);	
	AUGE_SAFE_RELEASE(pFeatureClass);
	
}

void BydTest::QueryTest_2()
{
	int counter = 1;
	while(true)
	{
		printf("\r%d", counter++);
		QueryTest();
	}
}

void BydTest::GetUniqueValue()
{
	auge::FeatureClass* pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	auge::EnumValue* pEnumValues = pFeatureClass->GetUniqueValue("country");

	auge::GValue* pValue = NULL;
	pEnumValues->Reset();
	while((pValue=pEnumValues->Next())!=NULL)
	{
		switch(pValue->GetType())
		{
		case augeValueTypeInt:
			printf("%d\n", pValue->GetInt());
			break;
		case augeValueTypeDouble:
			printf("%f\n", pValue->GetDouble());
			break;
		case augeValueTypeString:
			printf("%s\n", pValue->GetString());
			break;
		}
	}

	pEnumValues->Release();
	pFeatureClass->Release();
}

void BydTest::GetNullValue()
{
	auge::FeatureClass* pFeatureClass = m_pWorkspace->OpenFeatureClass("rivers");

	auge::GValue* pValue = NULL;
	auge::Feature* pFeature = NULL;
	auge::FeatureCursor* pCursor = pFeatureClass->Query();
	while((pFeature=pCursor->NextFeature())!=NULL)
	{
		if(pFeature->GetFID()==3)
		{
			pValue = pFeature->GetValue("system");
			pValue->Release();
		}

		pFeature->Release();
	}

	pCursor->Release();
	pFeatureClass->Release();
}

void BydTest::GetAQI()
{
	auge::FeatureClass* pFeatureClass = m_pWorkspace->OpenFeatureClass("gc_aqi_2");

	auge::GValue* pValue = NULL;
	auge::Feature* pFeature = NULL;
	auge::FeatureCursor* pCursor = pFeatureClass->Query();
	while((pFeature=pCursor->NextFeature())!=NULL)
	{
		
		pValue = pFeature->GetValue("time_point");
		TIME_STRU* tim = pValue->GetTime();
		pValue->Release();
		
		pFeature->Release();
	}

	pCursor->Release();
	pFeatureClass->Release();
}

void BydTest::GetAQIQuery()
{
	auge::FeatureClass* pFeatureClass = m_pWorkspace->OpenFeatureClass("gc_aqi_2");

	TIME_STRU tim;
	memset(&tim,0,sizeof(TIME_STRU));
	tim.usYear = 2015;
	tim.usMonth= 6;
	tim.usDay = 28;
	tim.usHour = 17;
	tim.usMinute = 0;
	tim.usSecond = 0;
	auge::GValue* pValue = new auge::GValue(&tim,true);

	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();
	auge::BinaryComparisonFilter* pFilter = pFactory->CreateBinaryComparisonFilter();
	auge::PropertyName* pPropName = pFactory->CreatePropertyName("time_point");
	auge::Literal* pLiteral = pFactory->CreateLiteral(pValue);
	pFilter->SetExpression1(pPropName);
	pFilter->SetExpression2(pLiteral);
	pFilter->SetOperator(auge::augeComOprEqual);

	//auge::GValue* pValue = NULL;
	auge::Feature* pFeature = NULL;
	auge::FeatureCursor* pCursor = pFeatureClass->Query(pFilter);
	while((pFeature=pCursor->NextFeature())!=NULL)
	{

		pValue = pFeature->GetValue("time_point");
		TIME_STRU* tim = pValue->GetTime();
		pValue->Release();

		pFeature->Release();
	}

	pCursor->Release();
	pFeatureClass->Release();
	pFilter->Release();
}

void BydTest::RefreshFeatureClass()
{
	auge::FeatureClass* pClass = NULL;
	auge::EnumDataSet* pFeatureClasses = m_pWorkspace->GetFeatureClasses();

	pFeatureClasses->Reset();
	while((pClass=(auge::FeatureClass*)(pFeatureClasses->Next()))!=NULL)
	{
		pClass->Refresh();
	}
	pFeatureClasses->Release();
}