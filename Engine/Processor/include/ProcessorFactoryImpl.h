#ifndef __AUGE_PROCESSPOR_FACTORY_IMPL_H__
#define __AUGE_PROCESSPOR_FACTORY_IMPL_H__

#include "AugeProcessor.h"

namespace auge
{
	class GProcessorFactoryImpl : public GProcessorFactory
	{
	public:
		GProcessorFactoryImpl();
		virtual ~GProcessorFactoryImpl();

	public:
		virtual MapTileStoreGenerator*		CreateTileStoreGenerator();
		virtual MapTileStoreGenerator*		CreateMongoTileStoreGenerator();
		virtual ArchiveProcessor*			CreateArchiveProcessor();
		virtual FeatureImportProcessor*		CreateFeatureImportProcessor();
		virtual FeatureExportProcessor*		CreateFeatureExportProcessor();

		virtual CsvImportProcessor*			CreateCsvImportProcessor();

		// Geometry
		virtual MultiPointToPointsProcessor* CreateMultiPointToPointsProcessor();
		virtual LineToPointsProcessor*		CreateLineToPointsProcessor();
		virtual PolygonToPointsProcessor*	CreatePolygonToPointsProcessor();
		virtual PolygonToLineProcessor*		CreatePolygonToLineProcessor();

		// Cluster 
		virtual KMeansProcessor*			CreateKMeansProcessor();

		// projection
		virtual FeatureProjectProcessor*	CreateFeatureProjectProcessor();

		// Raster
		virtual RasterGraylizeProcessor*			CreateRasterGraylizeProcessor();
		virtual RasterReverseProcessor*				CreateRasterReverseProcessor();

		virtual RasterExtractByRectangleProcessor*	CreateRasterExtractByRectangleProcessor();

		// ͼ����ǿ
		virtual RasterStretchProcessor*				CreateRasterStretchProcessor();
		virtual RasterEdgeDetectProcessor*			CreateRasterEdgeDetectProcessor();		
		virtual RasterSmoothProcessor*				CreateRasterSmoothProcessor();
		virtual RasterSubtractProcessor*			CreateRasterSubtractProcessor();
		virtual RasterHistogramEqualizationProcessor* CreateRasterHistogramEqualizationProcessor();

		// ͼ���ں�
		virtual RasterPixelBlendProcessor*			CreateRasterPixelBlendProcessor();
		// ͼ��ָ�
		virtual RasterThresholdProcessor*			CreateRasterThresholdProcessor();

		virtual RasterFormatConvertToJPEGProcessor*	CreateRasterFormatConvertToJPEGProcessor();
		// ͼ����Ч
		virtual RasterSepiaToneEffectProcessor*		CreateRasterSepiaToneEffectProcessor();
		virtual RasterPenEffectProcessor*			CreateRasterPenEffectProcessor();

		// DEM
		virtual DemSlopeProcessor*					CreateDemSlopeProcessor();
		virtual DemAspectProcessor*					CreateDemAspectProcessor();
	};
}



#endif //__AUGE_PROCESSPOR_FACTORY_IMPL_H__
