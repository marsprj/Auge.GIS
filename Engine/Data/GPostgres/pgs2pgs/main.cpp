#include <stdio.h>
#include <stdlib.h>
#include "AugeFeature.h"


int main(int argc, char** argv)
{
	const char* shpName = argv[1];
	const char* className = shpName;

	const char* pgs_constr = "SERVER=172.32.72.2;INSTANCE=5432;DATABASE=base;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
	const char* shp_constr = "SERVER=192.168.111.160;INSTANCE=5432;DATABASE=base;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
	
	RESULTCODE rc = AG_SUCCESS;
	auge::DataEngine* pshpEngine = NULL;
	auge::DataEngine* ppgsEngine = NULL;
	auge::DataEngineManager* pDataEngineManager = NULL;

	pDataEngineManager = auge::augeGetDataEngineManagerInstance();
	pDataEngineManager->Load();

	pshpEngine = pDataEngineManager->GetEngine("Postgres");
	ppgsEngine = pDataEngineManager->GetEngine("Postgres");

	auge::FeatureWorkspace* pshpWorkspace = NULL;
	auge::FeatureWorkspace* ppgsWorkspace = NULL;

	pshpWorkspace = dynamic_cast<auge::FeatureWorkspace*>(pshpEngine->CreateWorkspace());
	ppgsWorkspace = dynamic_cast<auge::FeatureWorkspace*>(ppgsEngine->CreateWorkspace());

	pshpWorkspace->SetConnectionString(shp_constr);
	ppgsWorkspace->SetConnectionString(pgs_constr);

	rc = pshpWorkspace->Open();
	rc = ppgsWorkspace->Open();

	//pshpWorkspace->SetUser(2);
	//ppgsWorkspace->SetUser(2);

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
		printf("\r\t%d:%d",counter++, count);

		cmd->Insert(pFeature);

		pFeature->Release();
	}
	cmd->Commit();
	cmd->Release();

	ppgsFeatureClass->Refresh();
	ppgsFeatureClass->BuildSpatialIndex();
	ppgsFeatureClass->Release();

	printf("\n");

	ppgsWorkspace->Release();
	pshpWorkspace->Release();
	return 0;
}