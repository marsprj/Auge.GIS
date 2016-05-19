#include "GDALTest.h"
#include "gdal/ogrsf_frmts.h"
#include <gdal/gdal_priv.h>
#include <fstream>
#include "wkb.h"

#define g_max(a,b)    (((a) > (b)) ? (a) : (b))
#define g_min(a,b)    (((a) < (b)) ? (a) : (b))

using namespace auge;

//CPPUNIT_TEST_SUITE_REGISTRATION(GDALTest);

bool IsNoDataPixel(float v);
bool IsPointInPolgyon(float x, float y, WKBPolygon* pWKBPolygon);

void GDALTest::setUp() 
{
	printf("setUp\n");
}

void GDALTest::tearDown()
{
	
}

void GDALTest::Read_shp()
{
	const char* shp_path = "H:\\temp\\xam\\Xinanmeng_Border.shp";

	OGRFeature* poFeature = NULL;
	OGRFeatureDefn* poFeatureDefn = NULL;
	OGRFieldDefn* poFieldDefn = NULL;
	OGRLayer* poLayer = NULL;
	OGRDataSource *poDataSource = NULL;
	OGRFieldType ftype = OFTInteger;
	int nField = 0;


	OGRRegisterAll();

	//打开shp文件
	poDataSource = OGRSFDriverRegistrar::Open(shp_path);
	if(poDataSource==NULL)
	{
		return;
	}

	poLayer = poDataSource->GetLayerByName("Xinanmeng_Border");
	//读取shp图层的字段定义
	poFeatureDefn = poLayer->GetLayerDefn();
	nField = poFeatureDefn->GetFieldCount();

	//重置poLayer的游标
	poLayer->ResetReading();
	//遍历shp文件上的图元，每个图元为一个Feature
	while((poFeature=poLayer->GetNextFeature())!=NULL)
	{
		for(int i=0; i<nField; i++)
		{
			//获得字段定义对象
			poFieldDefn = poFeatureDefn->GetFieldDefn(i);
			ftype = poFieldDefn->GetType();
			switch(ftype)
			{
			case OFTInteger:
				printf("%d\n", poFeature->GetFieldAsInteger(i));
				break;
			case OFTReal:
				printf("%d\n", poFeature->GetFieldAsDouble(i));
				break;
			case OFTString:
				printf("%d\n", poFeature->GetFieldAsString(i));
				break;
			}
		}

		//读取空间数据
		OGRGeometry* poGeometry = NULL;
		poGeometry = poFeature->GetGeometryRef();
		if(poGeometry!=NULL)
		{
			int wkbSize = poGeometry->WkbSize();
			unsigned char * wkb = (unsigned char*)malloc(wkbSize);
			memset(wkb, 0, wkbSize);
			poGeometry->exportToWkb(OGRwkbByteOrder::wkbNDR,wkb);
			WKBGeometryType wkbType = (WKBGeometryType)(WKBTYPE(wkb));
			WKBPolygon* pWKBPolygon = (WKBPolygon*)wkb;

			free(wkb);
		}


		//销毁poFeature对象
		OGRFeature::DestroyFeature( poFeature );
	}

	//关闭shp文件
	OGRDataSource::DestroyDataSource(poDataSource);
}

void GDALTest::Read_tif()
{
	const char*  tif_path = "H:\\temp\\xam\\Soil_Erosion_K_Factor.tif";

	GDALAllRegister();
	GDALDataset* poDataset = (GDALDataset*)GDALOpen(tif_path, GA_ReadOnly);	
	if(poDataset==NULL)
	{
		const char* msg = CPLGetLastErrorMsg();
		printf("%s\n", msg);
		return;
	}
	

	int w = poDataset->GetRasterXSize();
	int h = poDataset->GetRasterYSize();
	
	double geo_transform[6];
	poDataset->GetGeoTransform(geo_transform);
	double x0 = geo_transform[0];
	double y0 = geo_transform[3];
	double x1 = x0 + geo_transform[1] * w;
	double y1 = y0 + geo_transform[5] * h;
	double map_width = x1 - x0;
	double map_height= y1 - y0;

	int nBand = poDataset->GetRasterCount();
	GDALRasterBand* poBand = poDataset->GetRasterBand(1);
	GDALDataType pixelType = poBand->GetRasterDataType();

	float* data = (float*)malloc( w * h * sizeof(float));
	memset(data, 0, w*h*sizeof(float));

	CPLErr err = poBand->RasterIO(GF_Read,
								0,0,
								w,h,
								data,
								w,h,
								pixelType,
								0,0);
	if(err!=CE_None)
	{
		const char* msg = CPLGetLastErrorMsg();
		printf("%s\n", msg);
		return;
	}
	
	WKBPolygon* pWKBPolygon = NULL;

	float * ptr = data;
	for(int i=0; i<h; i++)
	{
		for(int j=0; j<w; j++, ptr++)
		{
			float v = *ptr;
			if(!IsNoDataPixel(v))
			{
				//像素坐标转换为屏幕坐标
				float x = x0 + map_width * ((double)j / (double)w);
				float y = y0 + map_height * ((double)(h-i) / (double)h);
				if(IsPointInPolgyon(x, y, pWKBPolygon))
				{
					//处理在多边形内的像素
				}
			}
		}
	}

	free(data);

	GDALClose(poDataset);
}

bool IsNoDataPixel(float v)
{
	return (fabs(v)<1e10);
}

bool IsPointInPolgyon(float x, float y, WKBPolygon* pWKBPolygon)
{
	g_uint counter = 0;

	int i=0, j=0;
	int numPoints = 0;
	int numRings  = pWKBPolygon->numRings;
	auge::LinearRing* pLinearRing = NULL;
	auge::Point *pt0 = NULL;
	auge::Point *pt1 = NULL;
	double xmax;
	double ymin, ymax;
	double v0, v1;

	pLinearRing = (auge::LinearRing*)(&(pWKBPolygon->rings[0]));
	for(i=0; i<numRings; i++)
	{
		numPoints = pLinearRing->numPoints;
		pt0 = (auge::Point*)(&(pLinearRing->points[0]));
		pt1 = pt0 + 1;

		for(j=1; j<numPoints; j++, pt0++, pt1++)
		{
			v0 = y - pt0->y;
			v1 = y - pt1->y;
			if((v0==0.0) && (v1==0.0))
			{	//horizonal segment
				counter++;
			}
			else
			{
				ymin = g_min(pt0->y, pt1->y);
				ymax = g_max(pt0->y, pt1->y);
				v0 = y - ymin;
				v1 = y - ymax;

				if( v0>0.0 && v1<0.0)
				{
					// x locates between y0 and y1, then x may be accross the segment [pt0, pt1];
					xmax = g_max(pt0->x, pt1->x);
					if(xmax > x)
					{	// intersection point locates the right side of x
						double xx = 0.0;
						double s1, s2;
						double height = 0.0;
						s1 = y - pt0->y;
						s2 = pt1->y - y;
						if(fabs(s1)<fabs(s2))
						{
							xx = (pt0->x-pt1->x) * ((y-pt1->y) / (pt0->y-pt1->y)) + pt1->x;
						}
						else
						{
							xx = (pt1->x-pt0->x) * (y-pt0->y) / (pt1->y-pt0->y) + pt0->x;
						}
						if(xx>x)
						{
							counter++;
						}
					}
				}
			}
		}

		pLinearRing = (auge::LinearRing*)(pt1);
	}

	bool contain = true;
	if(contain==0)
	{
		return false;
	}
	return (counter%2);
}

bool IsPointInPolgyon(float x, float y, WKBMultiPolygon* pWKBMultiPolygon)
{
	g_uint counter = 0;
	auge::Point* pt0 = NULL;
	auge::Point* pt1 = NULL; 
	double xmax;
	double ymin, ymax;
	double v0, v1;

	int numPolygons = pWKBMultiPolygon->numPolygons;
	WKBPolygon* pWKBPolygon = &(pWKBMultiPolygon->polygons[0]);
	for(int i=0; i<numPolygons; i++)
	{
		int numRings = pWKBPolygon->numRings;
		LinearRing* pLinearRing = &(pWKBPolygon->rings[0]);
		for(int j=0; j<numRings; j++)
		{	
			int numPoints = pLinearRing->numPoints;
			pt0 = &(pLinearRing->points[0]);
			pt1 = pt0 + 1;
			for(int k=1; k<numPoints; k++, pt0++, pt1++)
			{
				v0 = y - pt0->y;
				v1 = y - pt1->y;
				if((v0==0.0) && (v1==0.0))
				{	//horizonal segment
					counter++;
				}
				else
				{
					ymin = g_min(pt0->y, pt1->y);
					ymax = g_max(pt0->y, pt1->y);
					v0 = y - ymin;
					v1 = y - ymax;

					if( v0>0.0 && v1<0.0)
					{
						// x locates between y0 and y1, then x may be accross the segment [pt0, pt1];
						xmax = g_max(pt0->x, pt1->x);
						if(xmax > x)
						{	// intersection point locates the right side of x

							double xx = 0.0;
							double s1, s2;
							double height = 0.0;
							s1 = y - pt0->y;
							s2 = pt1->y - y;
							if(fabs(s1)<fabs(s2))
							{
								xx = (pt0->x-pt1->x) * ((y-pt1->y) / (pt0->y-pt1->y)) + pt1->x;
							}
							else
							{
								xx = (pt1->x-pt0->x) * (y-pt0->y) / (pt1->y-pt0->y) + pt0->x;
							}
							if(xx>x)
							{
								counter++;
							}

							//counter++;
						}
					}
				}
			}
			pLinearRing = (LinearRing*)pt1;
		}
		pWKBPolygon = (WKBPolygon*)pt1;
	}

	bool contain = true;
	if(contain==0)
	{
		return false;
	}
	return (counter%2);
}

void GDALTest::Show_Drivers()
{
	GDALAllRegister();
	GDALDriverManager* manager = GetGDALDriverManager();
	int count = manager->GetDriverCount();
	for(int i=0; i<count; i++)
	{
		GDALDriver* driver = manager->GetDriver(i);
		const char* can_create = driver->GetMetadataItem(GDAL_DCAP_CREATE);
		printf("[%s]:%s\n", driver->GetDescription(), can_create);
	}
}