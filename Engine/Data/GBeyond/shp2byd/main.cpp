#include <stdio.h>
#include <stdlib.h>
#include "AugeData.h"
#include "AugeFeature.h"

char* files[] = {
	//"ר��ҽԺ",
	//"�й���������",
	//"��ѧ",
	//"��Ҫ��·",
	//"��ͨ����",
	//"��ͨ��ʩ����",
	//"סլС��",
	//"ס�޷���",
	//"�������з���",
	//"���չ�˾",
	//"������ʩ",
	"��˾��ҵ",
	"���취����",
	"д��¥",
	"����ҽ�Ƴ���",
	"���ؼ�����",
	"���ؼ���������",
	"ҽ�Ʊ�������",
	"����ˮϵ",
	"����סլ",
	"����",
	"����������",
	"���м���������",
	"���й����ͨ",
	"Сѧ���׶�԰",
	"С·",
	"����·",
	"Ħ�г�����",
	"�������ؼ��������",
	"��",
	"ֲ��",
	"��Ҫ��·",
	"ˮϵ����",
	"��������",
	"����ά��",
	"��������",
	"��վ",
	"�������",
	"ʡ������",
	"ʡ����������",
	"�ƽ��Ļ�����",
	"�ۺ�ҽԺ",
	"�Զ�ȡ���",
	"��Ȼ����",
	"��������",
	"֤ȯ��˾",
	"����˾",
	"�������",
	"��·������ʩ",
	"���ڱ��ջ���",
	"��·",
	"����",
	"�������",
	"��״ˮϵ",
	"�羰��ʤ",
	"�ɻ���",
	"��������",
	"�ߵ�ԺУ",
	"����·"};

int main(int argc, char** argv)
{
	//const char* shpName = argv[1];
	//const char* className = argv[2];

	//const char* shp_constr = "DATABASE=G:\\Data\\shape\\osm";
	const char* shp_constr = "DATABASE=D:\\GeoBeans\\data\\world";
	//const char* shp_constr = "DATABASE=H:\\Data\\data\\�й�_WGS84\\ȫ��_shap\\�������� - ����";
	//const char* shp_constr = "DATABASE=G:\\temp\\ȫ��_shap";
	//const char* shp_constr = "DATABASE=G:\\Data\\shape\\tianjin\\������";
	//const char* shp_constr = "DATABASE=E:\\data\\";
	const char* byd_constr = "SERVER=127.0.0.1;INSTANCE=II;DATABASE=gisdb;USER=gisdb;PASSWORD=qwer1234";

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

	//const char* shpName = "�ߵ�ԺУ";
	//const char* className = "�ߵ�ԺУ";

	const char* shpName = "cities";
	const char* className = "cities_3";
	
	auge::DataEngine* pshpEngine = NULL;
	auge::DataEngine* pbydEngine = NULL;
	auge::DataEngineManager* pDataEngineManager = NULL;

	pDataEngineManager = auge::augeGetDataEngineManagerInstance();
	pDataEngineManager->Load();

	pshpEngine = pDataEngineManager->GetEngine("Shape");
	pbydEngine = pDataEngineManager->GetEngine("Beyond");

	auge::FeatureWorkspace* pshpWorkspace = NULL;
	auge::FeatureWorkspace* pbydWorkspace = NULL;

	pshpWorkspace = dynamic_cast<auge::FeatureWorkspace*>(pshpEngine->CreateWorkspace());
	pbydWorkspace = dynamic_cast<auge::FeatureWorkspace*>(pbydEngine->CreateWorkspace());

	pshpWorkspace->SetConnectionString(shp_constr);
	pbydWorkspace->SetConnectionString(byd_constr);

	rc = pshpWorkspace->Open();
	rc = pbydWorkspace->Open();

	//int count = sizeof(files) / sizeof(char*);
	//for(int i=0; i<count; i++)
	//{
	//	const char* shpName = files[i];
	//	const char* className = files[i];
		
		auge::Feature* pFeature = NULL;
		auge::FeatureClass* pshpFeatureClass = NULL;
		auge::FeatureClass* pbydFeatureClass = NULL;
		auge::FeatureCursor* pCursor = NULL;
		auge::FeatureInsertCommand* cmd = NULL;
		auge::GFields* pFields = NULL;

		pshpFeatureClass = pshpWorkspace->OpenFeatureClass(shpName);
		pFields = pshpFeatureClass->GetFields();

		pbydWorkspace->RemoveFeatureClass(className);
		pbydFeatureClass = pbydWorkspace->OpenFeatureClass(className);
		if(pbydFeatureClass==NULL)
		{
			pbydFeatureClass = pbydWorkspace->CreateFeatureClass(className, pFields);
		}

		int count = pshpFeatureClass->GetCount();
		int counter=1;
		pCursor = pshpFeatureClass->Query();
		cmd = pbydFeatureClass->CreateInsertCommand();
		while((pFeature=pCursor->NextFeature()))
		{
			printf("\r\t%d:%d",counter++, count);
			//printf("\r[%d:%s]%d:%d",i, shpName, counter++, count);
			cmd->Insert(pFeature);

			pFeature->Release();
		}
		cmd->Commit();
		cmd->Release();

		pbydFeatureClass->Release();

		printf("\n");
	//}

	pbydWorkspace->Release();
	pshpWorkspace->Release();
	return 0;
}