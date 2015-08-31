#include "RasterTest.h"
#include "AugeCore.h"
#include "AugeData.h"
#include "AugeCarto.h"
#include "AugeGeometry.h"
#include "AugeProcessor.h"
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION(RasterTest);

void RasterTest::setUp() 
{
	auge::GLogger	*pLogger = auge::augeGetLoggerInstance();
	pLogger->Initialize();

	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Load();

	pEngine = pEngineManager->GetEngine("Postgres");
	m_pConnection = pEngine->CreateConnection();
	m_pConnection->SetConnectionString("SERVER=127.0.0.1;INSTANCE=5432;DATABASE=auge;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	//m_pConnection->SetConnectionString("SERVER=192.168.111.160;INSTANCE=5432;DATABASE=auge;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	m_pConnection->Open();

	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	pConnManager->Initialize(m_pConnection);

	printf("setUp\n");
}

void RasterTest::tearDown()
{
	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	pConnManager->Unload();

	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Unload();

	//m_pConnection->Close();
	//m_pConnection->Release();
	m_pConnection = NULL;
	printf("tearDown\n");
}

void RasterTest::ExtractByRectangle()
{	
	DWORD ts = GetTickCount();

	auge::RasterExtractByRectangleProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterExtractByRectangleProcessor();

	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("Koala.png");
	pProcessor->SetInputPath("/s1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("Koala_clipped.png");
	pProcessor->SetOutputPath("/s2");

	auge::GEnvelope extent(100,100,500,500);
	pProcessor->SetInputRectangle(extent);

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();
	
	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::Stretch()
{
	DWORD ts = GetTickCount();

	auge::RasterStretchProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterStretchProcessor();

	pProcessor->SetInputDataSource("rsdb");
	pProcessor->SetInputRaster("srtm_58_05.tif");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("srtm_58_05.jpg");
	//pProcessor->SetOutputPath("/ds1");

	//auge::GColor start_color(0,255,0,255);
	//pProcessor->SetStartColor(start_color);
	//auge::GColor end_color(255,0,0,255);
	//pProcessor->SetEndColor(end_color);

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::EdgeDetorSobol()
{
	DWORD ts = GetTickCount();

	auge::RasterEdgeDetectProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterEdgeDetectProcessor();

	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("srtm_58_05.tif");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("srtm_58_05_edge.tif");

	//pProcessor->SetInputDataSource("rsdb2");
	//pProcessor->SetInputRaster("Koala.jpeg");

	//pProcessor->SetOutputDataSource("rsdb2");
	//pProcessor->SetOutputRaster("Koala_edge.jpeg");

	pProcessor->SetEdgeDetector(auge::augeEdgeSobel);

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::Graylize()
{
	DWORD ts = GetTickCount();

	auge::RasterGraylizeProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterGraylizeProcessor();

	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("Desert.jpg");
	pProcessor->SetInputPath("/ds1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("gray.jpg");
	pProcessor->SetOutputPath("/ds1");

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::ToJPG()
{
	DWORD ts = GetTickCount();

	auge::RasterFormatConvertToJPEGProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterFormatConvertToJPEGProcessor();

	//pProcessor->SetInputDataSource("rsdb2");
	//pProcessor->SetInputRaster("srtm_58_05.tif");

	//pProcessor->SetOutputDataSource("rsdb2");
	//pProcessor->SetOutputRaster("srtm_58_05.jpeg");

	auge::GEnvelope extent;
	extent.Set(100,100,800,800);

	pProcessor->SetInputRectangle(extent);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("Koala.png");
	pProcessor->SetInputPath("/s1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("Koala_xxx.jpg");
	//pProcessor->SetInputPath("/s1");

	pProcessor->SetRed(1);
	pProcessor->SetGreen(0);
	pProcessor->SetBlue(0);

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::Reverse()
{
	DWORD ts = GetTickCount();

	auge::RasterReverseProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterReverseProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("Desert.jpg");
	pProcessor->SetInputPath("/s1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("Desert_reverse.jpg");
	pProcessor->SetOutputPath("/s2/s_1_2");

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::Smooth()
{
	DWORD ts = GetTickCount();

	auge::RasterSmoothProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterSmoothProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("Desert.jpg");
	pProcessor->SetInputPath("/s1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("Desert_mean.jpg");
	pProcessor->SetOutputPath("/s2/s_1_2");
	//pProcessor->SetOutputRaster("Desert_gauss.jpg");

	//pProcessor->SetSmoother(auge::augeSmoothMean);
	//pProcessor->SetSmoother(auge::augeSmoothGauss);
	pProcessor->SetSmoother(auge::augeSmoothMedian);

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::Subtract()
{
	DWORD ts = GetTickCount();

	auge::RasterSubtractProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterSubtractProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource_1("rsdb2");
	pProcessor->SetInputRaster_1("Desert.jpg");
	pProcessor->SetInputPath_1("/s1");

	pProcessor->SetInputDataSource_2("rsdb2");
	pProcessor->SetInputRaster_2("Desert_mean.jpg");
	pProcessor->SetInputPath_2("/s2/s_1_2");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("Subtract.jpg");
	pProcessor->SetOutputPath("/s2/s_1_2");

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::PixelBlend()
{
	DWORD ts = GetTickCount();

	auge::RasterPixelBlendProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterPixelBlendProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource_1("rsdb2");
	pProcessor->SetInputRaster_1("Desert.jpg");
	pProcessor->SetInputPath_1("/ds1");

	pProcessor->SetInputDataSource_2("rsdb2");
	pProcessor->SetInputRaster_2("Koala.jpg");
	pProcessor->SetInputPath_2("/ds1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("Desert_Koala_5.jpg");
	pProcessor->SetOutputPath("/ds1");

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::HistgramEqualization()
{
	DWORD ts = GetTickCount();

	auge::RasterHistogramEqualizationProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterHistogramEqualizationProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("gray.jpg");
	pProcessor->SetInputPath("/ds1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("gray_histeql.jpg");
	pProcessor->SetOutputPath("/ds1");
	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::Threshold()
{
	DWORD ts = GetTickCount();

	auge::RasterThresholdProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterThresholdProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("Desert.jpg");
	pProcessor->SetInputPath("/ds1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("Desert_threshold_average.jpg");
	pProcessor->SetOutputPath("/ds1");
	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::SepiaTone()
{
	DWORD ts = GetTickCount();

	auge::RasterSepiaToneEffectProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterSepiaToneEffectProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("Koala.jpg");
	pProcessor->SetInputPath("/ds1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("Koala_Sepia_2.jpg");
	pProcessor->SetOutputPath("/ds1");
	//pProcessor->SetOutputRaster("Desert_gauss.jpg");

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::PenEffect()
{
	DWORD ts = GetTickCount();

	auge::RasterPenEffectProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterPenEffectProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("Koala.jpg");
	pProcessor->SetInputPath("/ds1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("Koala_pen.jpg");
	pProcessor->SetOutputPath("/ds1");
	//pProcessor->SetOutputRaster("Desert_gauss.jpg");

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::DEMSlope()
{
	DWORD ts = GetTickCount();

	auge::DemSlopeProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateDemSlopeProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("srtm_58_05.tif");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("srtm_58_05_slope_3.tif");
	pProcessor->SetOutputPath("/s2/s_1_2");

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::DEMAspect()
{
	DWORD ts = GetTickCount();

	auge::DemAspectProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateDemAspectProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("srtm_58_05.tif");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("srtm_58_05_aspect.tif");
	pProcessor->SetOutputPath("/s2/s_1_2");

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::DEMHillShade()
{
	DWORD ts = GetTickCount();

	auge::DemHillshadeProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateDemHillshadeProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("ASTGTM2_N29E082_dem.tif");
	pProcessor->SetInputPath("/dem");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("n39_hillshade_7.tif");
	pProcessor->SetOutputPath("/dem_result");

	pProcessor->SetAzimuth(135);
	pProcessor->SetZenith(45);
	pProcessor->SetZFactor(5.0);

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}