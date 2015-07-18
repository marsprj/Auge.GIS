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

		virtual CsvImportProcessor*			CreateCsvImportProcessor();

		virtual KMeansProcessor*			CreateKMeansProcessor();

		virtual FeatureProjectProcessor*	CreateFeatureProjectProcessor();

		virtual RasterExtractByRectangleProcessor*	CreateRasterExtractByRectangleProcessor();
		virtual RasterStretchProcessor*				CreateRasterStretchProcessor();
		virtual RasterEdgeDetectProcessor*			CreateRasterEdgeDetectProcessor();
		virtual RasterGraylizeProcessor*			CreateRasterGraylizeProcessor();
		virtual RasterReverseProcessor*				CreateRasterReverseProcessor();
		virtual RasterSmoothProcessor*				CreateRasterSmoothProcessor();
		virtual RasterSubtractProcessor*			CreateRasterSubtractProcessor();
		virtual RasterFormatConvertToJPEGProcessor*	CreateRasterFormatConvertToJPEGProcessor();
	};
}



#endif //__AUGE_PROCESSPOR_FACTORY_IMPL_H__
