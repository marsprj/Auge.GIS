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

		virtual KMeanProcessor*				CreateKMeanProcessor();

		virtual FeatureProjectProcessor*	CreateFeatureProjectProcessor();

		virtual RasterExtractByRectangleProcessor*	CreateRasterExtractByRectangleProcessor();
		virtual RasterStretchProcessor*				CreateRasterStretchProcessor();
		virtual RasterEdgeDetectProcessor*			CreateRasterEdgeDetectProcessor();
		virtual RasterGreylizeProcessor*			CreateRasterGreylizeProcessor();
	};
}



#endif //__AUGE_PROCESSPOR_FACTORY_IMPL_H__
