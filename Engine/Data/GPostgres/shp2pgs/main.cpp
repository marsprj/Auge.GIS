#include <stdio.h>
#include <stdlib.h>
#include "AugeFeature.h"

char* files[] = {
	//"专科医院",
	//"中国行政区划",
	//"中学",
	//"主要道路",
	//"交通地名",
	//"交通设施服务",
	//"住宅小区",
	//"住宿服务",
	//"体育休闲服务",
	//"保险公司",
	//"公共设施",
	"公司企业",
	"公检法机关",
	"写字楼",
	"动物医疗场所",
	"区县级政府",
	"区县级行政区划",
	"医疗保健服务",
	"单线水系",
	"商务住宅",
	"国道",
	"地区市政府",
	"地市级行政区划",
	"城市轨道交通",
	"小学及幼儿园",
	"小路",
	"快速路",
	"摩托车服务",
	"政府机关及社会团体",
	"桥",
	"植被",
	"次要道路",
	"水系名称",
	"汽车服务",
	"汽车维修",
	"汽车销售",
	"火车站",
	"生活服务",
	"省级政府",
	"省级行政区划",
	"科教文化服务",
	"综合医院",
	"自动取款机",
	"自然地名",
	"行政地名",
	"证券公司",
	"财务公司",
	"购物服务",
	"道路附属设施",
	"金融保险机构",
	"铁路",
	"银行",
	"银行相关",
	"面状水系",
	"风景名胜",
	"飞机场",
	"餐饮服务",
	"高等院校",
	"高速路"};

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
	const char* pgs_constr = "SERVER=192.168.111.160;INSTANCE=5432;DATABASE=gaode;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";

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

	//const char* shpName = "高等院校";
	//const char* className = "高等院校";
	
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

	int count = sizeof(files) / sizeof(char*);
	for(int i=0; i<count; i++)
	{
		const char* shpName = files[i];
		const char* className = files[i];
		
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
			//printf("\r\t%d:%d",counter++, count);
			printf("\r[%d:%s]%d:%d",i, shpName, counter++, count);
			cmd->Insert(pFeature);

			pFeature->Release();
		}
		cmd->Commit();
		cmd->Release();

		ppgsFeatureClass->Release();

		printf("\n");
	}

	ppgsWorkspace->Release();
	pshpWorkspace->Release();
	return 0;
}