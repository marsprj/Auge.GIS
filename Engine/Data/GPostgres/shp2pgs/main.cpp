#include <stdio.h>
#include <stdlib.h>
#include "AugeFeature.h"

int main(int argc, char** argv)
{
	//const char* shpName = argv[1];
	//const char* className = argv[2];

	//const char* shp_constr = "DATABASE=G:\\Data\\shape\\osm";
	//const char* shp_constr = "DATABASE=D:\\GeoBeans\\data\\world";
	//const char* shp_constr = "DATABASE=H:\\Data\\data\\中国_WGS84\\全国_shap\\行政区划 - 副本";
	//const char* shp_constr = "DATABASE=G:\\temp\\全国_shap";
	//const char* shp_constr = "DATABASE=G:\\Data\\shape\\tianjin\\建筑物";
	const char* shp_constr = "DATABASE=E:\\data\\";
	//const char* pgs_constr = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
	//const char* pgs_constr = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=ourgis;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
	//const char* pgs_constr = "SERVER=192.168.111.160;INSTANCE=5432;DATABASE=osm;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
	//const char* pgs_constr = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=tianjin;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
	const char* pgs_constr = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gaode;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";

	RESULTCODE rc = AG_FAILURE;
	//const char* className = "cities";
	//const char* className = "rivers";
	//const char* className = "country";
	//const char* shpName = "cities";
	//const char* className = "cities";
	//const char* shpName = "World_water_os";
	//const char* className = "World_water_os";
	//const char* shpName = "World_coastline_os";
	//const char* className = "World_coastline_os";

	const char* shpName = "高等院校";
	const char* className = "高等院校";
	
	auge::DataEngine* pshpEngine = NULL;
	auge::DataEngine* ppgsEngine = NULL;
	auge::DataEngineManager* pDataEngineManager = NULL;

	pDataEngineManager = auge::augeGetDataEngineManagerInstance();
	pDataEngineManager->Load();

	pshpEngine = pDataEngineManager->GetEngine("Shape");
	ppgsEngine = pDataEngineManager->GetEngine("Postgres");

	auge::FeatureWorksapce* pshpWorkspace = NULL;
	auge::FeatureWorksapce* ppgsWorkspace = NULL;

	pshpWorkspace = dynamic_cast<auge::FeatureWorksapce*>(pshpEngine->CreateWorkspace());
	ppgsWorkspace = dynamic_cast<auge::FeatureWorksapce*>(ppgsEngine->CreateWorkspace());

	pshpWorkspace->SetConnectionString(shp_constr);
	ppgsWorkspace->SetConnectionString(pgs_constr);

	rc = pshpWorkspace->Open();
	rc = ppgsWorkspace->Open();

	auge::Feature* pFeature = NULL;
	auge::FeatureClass* pshpFeatureClass = NULL;
	auge::FeatureClass* ppgsFeatureClass = NULL;
	auge::FeatureCursor* pCursor = NULL;
	auge::FeatureInsertCommand* cmd = NULL;
	auge::GFields* pFields = NULL;

	pshpFeatureClass = pshpWorkspace->OpenFeatureClass(shpName);
	pFields = pshpFeatureClass->GetFields();

	ppgsWorkspace->RemoveFeatureClass(className);
	ppgsFeatureClass = ppgsWorkspace->OpenFeatureClass(className);
	if(ppgsFeatureClass==NULL)
	{
		ppgsFeatureClass = ppgsWorkspace->CreateFeatureClass(className, pFields);
	}
	
	int count = pshpFeatureClass->GetCount();
	int counter=1;
	pCursor = pshpFeatureClass->Query();
	cmd = ppgsFeatureClass->CreateInsertCommand();
	while((pFeature=pCursor->NextFeature()))
	{
		printf("\r%d:%d",counter++, count);
		cmd->Insert(pFeature);

		pFeature->Release();
	}
	cmd->Commit();
	cmd->Release();

	ppgsFeatureClass->Release();
	ppgsWorkspace->Release();
	pshpWorkspace->Release();
	return 0;
}