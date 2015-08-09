#ifndef __AUGE_GEO_PROCESSOR_H__
#define __AUGE_GEO_PROCESSOR_H__

#include "AugeCore.h"
#include "AugeObject.h"

#ifdef WIN32
#	ifdef AUGE_PROCESSOR_EXPORTS
#		define AUGE_PROCESSOR_API __declspec(dllexport)
#	else
#		define AUGE_PROCESSOR_API __declspec(dllimport)
#	endif
#else
#	define AUGE_PROCESSOR_API
#endif

namespace auge
{
	class Map;
	class GEnvelope;
	class TileStore;
	class Raster;
	class Geometry;
	class User;

	typedef enum augeEdgeDetector
	{
		augeEdgeSobel	=  0,
		augeEdgeRoberts		,
		augeEdgePrewitt		,
		augeEdgeLaplacian
	}augeEdgeDetector;

	typedef enum augeRasterSmoother
	{
		augeSmoothGauss	=	0,		//高斯滤波
		augeSmoothMean,				//均值滤波
		augeSmoothMedian,			//中值滤波
		augeSmoothBilateral			//双边滤波
	}augeRasterSmoother;

	class GProcessor : public GObject
	{
	protected:
		GProcessor(){}
		virtual ~GProcessor(){}

	public:		
		virtual RESULTCODE	Execute() = 0;
		virtual void		SetUser(g_uint user) = 0;
	};

	class BufferProcessor : public GProcessor
	{
	protected:
		BufferProcessor(){}
		virtual ~BufferProcessor(){}
	public:
		virtual	void		SetGeometry(Geometry* pGeometry) = 0;
		virtual Geometry*	GetGeometry() = 0;
		
		virtual void		SetRadius(double radius) = 0;
		virtual double		GetRadius() = 0;

		virtual Geometry*	GetResultGeometry() = 0;
	};

	class CsvImportProcessor : public GProcessor
	{
	protected:
		CsvImportProcessor(){}
		virtual ~CsvImportProcessor(){}
	public:
		virtual	void		SetCvsPath(const char* path) = 0;
		virtual void		SetDataSource(const char* sourceName) = 0;
		virtual void		SetDatasetName(const char* datasetName) = 0;
	};

	class MapTileStoreGenerator : public GProcessor
	{
	protected:
		MapTileStoreGenerator(){}
		virtual ~MapTileStoreGenerator(){}
	public:
		virtual	void	SetMap(Map* pMap) = 0;
		virtual void	SetTileStore(TileStore* pTileStore) = 0;
		virtual void	SetStartLevel(g_uint l) = 0;
		virtual void	SetEndLevel(g_uint l) = 0;
		virtual void	SetViewer(GEnvelope& viewer) = 0;
	};
	
	class RasterTileStoreGenerator : public GProcessor
	{
	protected:
		RasterTileStoreGenerator(){}
		virtual ~RasterTileStoreGenerator(){}
	public:
		virtual	void	SetRaster(Raster* pRaster) = 0;
		virtual void	SetFormat(const char* format) = 0;
		virtual void	SetTileStore(TileStore* pTileStore) = 0;
		virtual void	SetStartLevel(g_uint l) = 0;
		virtual void	SetEndLevel(g_uint l) = 0;
		virtual void	SetViewer(GEnvelope& viewer) = 0;
	};

	class ArchiveProcessor : public GProcessor
	{
	protected:
		ArchiveProcessor(){}
		virtual ~ArchiveProcessor(){}
	public:

		virtual RESULTCODE	Compress() = 0;
		virtual RESULTCODE	Decompress() = 0;

		virtual void		AddFile(const char* fpath) = 0;
		virtual void		SetArchivePath(const char* apath) = 0;
		virtual void		Clear() = 0;

		virtual void		Release() = 0;
	};

	class FeatureImportProcessor : public GProcessor
	{
	protected:
		FeatureImportProcessor(){}
		virtual ~FeatureImportProcessor(){}
	public:
		virtual void		SetShapePath(const char* path) = 0;
		virtual void		SetShapeName(const char* className) = 0;
		virtual void		SetDataSourceName(const char* sourceName) = 0;
		virtual void		SetTypeName(const char* typeName) = 0;
	};

	//------------------------------------------------------------------------
	// Raster Processor Begin
	//------------------------------------------------------------------------
	class RasterExtractByRectangleProcessor : public GProcessor
	{
	protected:
		RasterExtractByRectangleProcessor(){}
		virtual ~RasterExtractByRectangleProcessor(){}
	public:
		virtual void		SetInputRectangle(GEnvelope& rect) = 0;

		virtual void		SetInputDataSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;
	};

	class RasterStretchProcessor : public GProcessor
	{
	protected:
		RasterStretchProcessor(){}
		virtual ~RasterStretchProcessor(){}
	public:
		virtual void		SetInputDataSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;

		virtual	void		SetStartColor(GColor color) = 0;
		virtual	void		SetEndColor(GColor color) = 0;
	};

	/**
	 * RasterGraylizeProcessor
	 *
	 * RGB转灰度图
	 */
	class RasterGraylizeProcessor : public GProcessor
	{
	protected:
		RasterGraylizeProcessor(){}
		virtual ~RasterGraylizeProcessor(){}
	public:
		virtual void		SetInputDataSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;
	};

	/**
	 * RasterSmoothProcessor
	 *
	 * 图像平滑
	 */
	class RasterSmoothProcessor : public GProcessor
	{
	protected:
		RasterSmoothProcessor(){}
		virtual ~RasterSmoothProcessor(){}
	public:
		virtual void		SetInputDataSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;

		virtual void		SetSmoother(augeRasterSmoother smoother) = 0;
	};

	/**
	 * RasterEdgeDetectProcessor
	 *
	 * 图像边缘提取
	 */
	class RasterEdgeDetectProcessor : public GProcessor
	{
	protected:
		RasterEdgeDetectProcessor(){}
		virtual ~RasterEdgeDetectProcessor(){}
	public:
		virtual void		SetInputDataSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;

		virtual void		SetEdgeDetector(augeEdgeDetector detector) = 0;
	};
	
	/**
	 * RasterResampleProcessor
	 *
	 * 图像重采样
	 */
	class RasterResampleProcessor : public GProcessor
	{
	protected:
		RasterResampleProcessor(){}
		virtual ~RasterResampleProcessor(){}
	public:
		virtual void		SetInputDataSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;
	};

	/**
	 * RasterBinarizationProcessor
	 *
	 * 图像二值化
	 */
	class RasterBinarizationProcessor : public GProcessor
	{
	protected:
		RasterBinarizationProcessor(){}
		virtual ~RasterBinarizationProcessor(){}
	public:
		virtual void		SetInputDataSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;
	};

	/**
	 * RasterReverseProcessor
	 *
	 * 图像反转
	 */
	class RasterReverseProcessor : public GProcessor
	{
	protected:
		RasterReverseProcessor(){}
		virtual ~RasterReverseProcessor(){}
	public:
		virtual void		SetInputDataSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;
	};

	/**
	 * RasterSubstractProcessor
	 *
	 * 图像相减
	 */
	class RasterSubtractProcessor : public GProcessor
	{
	protected:
		RasterSubtractProcessor(){}
		virtual ~RasterSubtractProcessor(){}
	public:
		virtual void		SetInputDataSource_1(const char* sourceName) = 0;
		virtual void		SetInputRaster_1(const char* rasterName) = 0;
		virtual void		SetInputPath_1(const char* rasterPath) = 0;

		virtual void		SetInputDataSource_2(const char* sourceName) = 0;
		virtual void		SetInputRaster_2(const char* rasterName) = 0;
		virtual void		SetInputPath_2(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;
	};

	class RasterSepiaToneEffectProcessor : public GProcessor
	{
	protected:
		RasterSepiaToneEffectProcessor(){}
		virtual ~RasterSepiaToneEffectProcessor(){}
	public:
		virtual void		SetInputDataSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;

		virtual RESULTCODE	Execute() = 0;
		virtual void		Release() = 0;
	};

	class RasterFormatConvertToJPEGProcessor : public GProcessor
	{
	protected:
		RasterFormatConvertToJPEGProcessor(){}
		virtual ~RasterFormatConvertToJPEGProcessor(){}
	public:
		virtual void		SetInputRectangle(GEnvelope& rect) = 0;

		virtual void		SetInputDataSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* path) = 0;

		virtual void		SetRed(g_uint i) = 0;
		virtual void		SetGreen(g_uint i) = 0;
		virtual void		SetBlue(g_uint i) = 0;
	};

	/**
	 * DemSlopeProcessor
	 *
	 * DEM坡度
	 */
	class DemSlopeProcessor : public GProcessor
	{
	protected:
		DemSlopeProcessor(){}
		virtual ~DemSlopeProcessor(){}
	public:
		virtual void		SetInputDataSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;
	};

	/**
	 * DemAspectProcessor
	 *
	 * DEM坡度
	 */
	class DemAspectProcessor : public GProcessor
	{
	protected:
		DemAspectProcessor(){}
		virtual ~DemAspectProcessor(){}
	public:
		virtual void		SetInputDataSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;
	};

	//------------------------------------------------------------------------
	// Raster Processor Begin
	//------------------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	// Projection Processor
	//////////////////////////////////////////////////////////////////////////
	class FeatureProjectProcessor : public GProcessor
	{
	protected:
		FeatureProjectProcessor(){}
		virtual ~FeatureProjectProcessor(){}
	public:
		virtual	void		SetInputDataSource(const char* sourceName) = 0;
		virtual	void		SetInputFeatureClass(const char* className) = 0;

		virtual	void		SetOutputDataSource(const char* sourceName) = 0;
		virtual	void		SetOutputFeatureClass(const char* className) = 0;
		virtual void		SetOutputSRID(g_uint srid) = 0;

		virtual RESULTCODE	Execute() = 0;
		virtual void		Release() = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	// Projection Processor
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Cluster Processor
	//////////////////////////////////////////////////////////////////////////
	class KMeansProcessor : public GProcessor
	{
	protected:
		KMeansProcessor(){}
		virtual ~KMeansProcessor(){}
	public:

		virtual void		SetK(g_uint k) = 0;

		virtual	void		SetInputDataSource(const char* sourceName) = 0;
		virtual	void		SetInputFeatureClass(const char* className) = 0;

		virtual	void		SetOutputDataSource(const char* sourceName) = 0;
		virtual	void		SetOutputFeatureClass(const char* className) = 0;

		virtual RESULTCODE	Execute() = 0;
		virtual void		Release() = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	// Cluster Processor
	//////////////////////////////////////////////////////////////////////////

	class GProcessorFactory
	{
	protected:
		GProcessorFactory(){}
		virtual ~GProcessorFactory(){}
	public:
		virtual MapTileStoreGenerator*		CreateTileStoreGenerator() = 0;
		virtual MapTileStoreGenerator*		CreateMongoTileStoreGenerator() = 0;
		virtual ArchiveProcessor*			CreateArchiveProcessor() = 0;
		virtual FeatureImportProcessor*		CreateFeatureImportProcessor() = 0;

		virtual CsvImportProcessor*			CreateCsvImportProcessor() = 0;

		// Cluster 
		virtual KMeansProcessor*			CreateKMeansProcessor() = 0;

		// projection
		virtual FeatureProjectProcessor*	CreateFeatureProjectProcessor() = 0;

		// Raster
		virtual RasterExtractByRectangleProcessor*	CreateRasterExtractByRectangleProcessor() = 0;
		virtual RasterStretchProcessor*				CreateRasterStretchProcessor() = 0;
		virtual RasterEdgeDetectProcessor*			CreateRasterEdgeDetectProcessor() = 0;
		virtual RasterGraylizeProcessor*			CreateRasterGraylizeProcessor() = 0;
		virtual RasterReverseProcessor*				CreateRasterReverseProcessor() = 0;
		virtual RasterSmoothProcessor*				CreateRasterSmoothProcessor() = 0;
		virtual RasterSubtractProcessor*			CreateRasterSubtractProcessor() = 0;
		virtual RasterFormatConvertToJPEGProcessor*	CreateRasterFormatConvertToJPEGProcessor() = 0;
		virtual RasterSepiaToneEffectProcessor*		CreateRasterSepiaToneEffectProcessor() = 0;

		// DEM
		virtual DemSlopeProcessor*					CreateDemSlopeProcessor() = 0;
		virtual DemAspectProcessor*					CreateDemAspectProcessor() = 0;
	};
	
	extern "C"
	{
		AUGE_PROCESSOR_API GProcessorFactory*	augeGetGeoProcessorFactoryInstance();
	}
}

//==============================================================================
//------------------------------------------------------------------------------
// 连接所需动态库
//------------------------------------------------------------------------------
#ifdef WIN32
#	ifndef AUGE_PROCESSOR_EXPORTS
#		define AUGE_PROCESSOR_NAME "GProcessor"
#		if defined(_DEBUG)
#			if defined(_UNICODE)
//#				define _AUGE_FILTER_CONFIGURATION "UD"
#				define _AUGE_FILTER_CONFIGURATION "D"
#			else
#				define _AUGE_FILTER_CONFIGURATION "D"
#			endif // _UNICODE
#		else
#			if defined(_UNICODE)
#				define _AUGE_FILTER_CONFIGURATION "U"
#			else
#				define _AUGE_FILTER_CONFIGURATION ""
#			endif // _UNICODE
#		endif // _DEBUG
#		pragma comment(lib, AUGE_PROCESSOR_NAME _AUGE_FILTER_CONFIGURATION ".lib")
#		pragma message("Automatically linking with " AUGE_PROCESSOR_NAME _AUGE_FILTER_CONFIGURATION ".dll")
#		undef _AUGE_FILTER_CONFIGURATION
#		undef AUGE_PROCESSOR_NAME
#	endif // AUGE_FILTER_EXPORTS 
#endif
//==============================================================================


#endif //__AUGE_GEO_PROCESSOR_H__
