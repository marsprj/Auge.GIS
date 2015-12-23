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
	class GMultiPoint;
	class GFilter;
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

	typedef enum augeRasterThresholder
	{
		augeThresholderAverage = 0,	//灰度均值

	}augeRasterThresholder;

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
		virtual	void		SetCsvPath(const char* path) = 0;
		virtual void		SetDataSource(const char* sourceName) = 0;
		virtual void		SetDatasetName(const char* datasetName) = 0;
	};

	class XYImportProcessor : public GProcessor
	{
	protected:
		XYImportProcessor(){}
		virtual ~XYImportProcessor(){}
	public:
		virtual	void		SetXYPath(const char* path) = 0;
		virtual void		SetDataSource(const char* sourceName) = 0;
		virtual void		SetDatasetName(const char* datasetName) = 0;
		virtual void		SetX(const char* fname) = 0;
		virtual void		SetY(const char* fname) = 0;
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
		virtual void		SetFeatureClassName(const char* className) = 0;
	};

	class FeatureExportProcessor : public GProcessor
	{
	protected:
		FeatureExportProcessor(){}
		virtual ~FeatureExportProcessor(){}
	public:
		virtual void		SetShapePath(const char* path) = 0;
		virtual void		SetShapeName(const char* className) = 0;
		virtual void		SetDataSourceName(const char* sourceName) = 0;
		virtual void		SetFeatureClassName(const char* className) = 0;
	};

	//------------------------------------------------------------------------
	// Geometry Processor Begin
	//------------------------------------------------------------------------
	/**
	 *
	 * MultiPointToPointsProcessor
	 * 
	 * 多点转换为单点
	 */
	class MultiPointToPointsProcessor : public GProcessor
	{
	protected:
		MultiPointToPointsProcessor(){}
		virtual ~MultiPointToPointsProcessor(){}
	public:
		virtual	void		SetInputDataSource(const char* sourceName) = 0;
		virtual	void		SetInputFeatureClass(const char* className) = 0;

		virtual	void		SetOutputDataSource(const char* sourceName) = 0;
		virtual	void		SetOutputFeatureClass(const char* className) = 0;
	};

	/**
	 *
	 * LineToPointsProcessor
	 * 
	 * 线转换为点
	 */
	class LineToPointsProcessor : public GProcessor
	{
	protected:
		LineToPointsProcessor(){}
		virtual ~LineToPointsProcessor(){}
	public:
		virtual	void		SetInputDataSource(const char* sourceName) = 0;
		virtual	void		SetInputFeatureClass(const char* className) = 0;

		virtual	void		SetOutputDataSource(const char* sourceName) = 0;
		virtual	void		SetOutputFeatureClass(const char* className) = 0;
	};

	/**
	 *
	 * PolygonToPointsProcessor
	 * 
	 * 多边形转换为点
	 */
	class PolygonToPointsProcessor : public GProcessor
	{
	protected:
		PolygonToPointsProcessor(){}
		virtual ~PolygonToPointsProcessor(){}
	public:
		virtual	void		SetInputDataSource(const char* sourceName) = 0;
		virtual	void		SetInputFeatureClass(const char* className) = 0;

		virtual	void		SetOutputDataSource(const char* sourceName) = 0;
		virtual	void		SetOutputFeatureClass(const char* className) = 0;
	};

	/**
	 *
	 * PolygonToLineProcessor
	 * 
	 * 多边形转换为线
	 */
	class PolygonToLineProcessor : public GProcessor
	{
	protected:
		PolygonToLineProcessor(){}
		virtual ~PolygonToLineProcessor(){}
	public:
		virtual	void		SetInputDataSource(const char* sourceName) = 0;
		virtual	void		SetInputFeatureClass(const char* className) = 0;

		virtual	void		SetOutputDataSource(const char* sourceName) = 0;
		virtual	void		SetOutputFeatureClass(const char* className) = 0;
	};

	/**
	 * 多边形的parts分解为独立多边形
	 */
	class PartsToPolygonsProcessor : public GProcessor
	{
	protected:
		PartsToPolygonsProcessor(){}
		virtual ~PartsToPolygonsProcessor(){}
	};

	/**
	 *　RandomPointsGenerator
	 ×　
	 * 随机点生成器
	 */
	class RandomPointsGenerator : public GProcessor
	{
	protected:
		RandomPointsGenerator(){}
		virtual ~RandomPointsGenerator(){}
	public:
		virtual	void		SetOutputDataSource(const char* sourceName) = 0;
		virtual	void		SetOutputFeatureClass(const char* className) = 0;
		virtual void		SetSRID(g_uint srid) = 0;
		virtual void		SetExtent(GEnvelope& extent) = 0;
		virtual void		SetCount(g_uint count) = 0;
	};

	class RandomPointsInPolygonGenerator : public GProcessor
	{
	protected:
		RandomPointsInPolygonGenerator(){}
		virtual ~RandomPointsInPolygonGenerator(){}
	public:
		virtual	void		SetInputDataSource(const char* sourceName) = 0;
		virtual	void		SetInputFeatureClass(const char* className) = 0;

		virtual	void		SetOutputDataSource(const char* sourceName) = 0;
		virtual	void		SetOutputFeatureClass(const char* className) = 0;

		virtual void		SetCount(g_uint count) = 0;

		virtual RESULTCODE	Execute() = 0;
		virtual GMultiPoint* Execute(Geometry* pGeometry, g_uint count) = 0;
	};

	/**
	 * 提取位于多边形内的点
	 */
	class ClipPointWithPoygon : public GProcessor
	{
	protected:
		ClipPointWithPoygon(){}
		virtual ~ClipPointWithPoygon(){}
	};

	/**
	 * 线融合
	 */
	class LineDissoveProcessor : public GProcessor
	{
	protected:
		LineDissoveProcessor(){}
		virtual ~LineDissoveProcessor(){}
	};

	/**
	 * 线化简
	 */
	class LineSimplificationProcessor : public GProcessor
	{
	protected:
		LineSimplificationProcessor(){}
		virtual ~LineSimplificationProcessor(){}

	};

	/**
	 * 多个多边形合并成一个
	 */
	class PolygonDissolveProcessor : public GProcessor
	{
	protected:
		PolygonDissolveProcessor(){}
		virtual ~PolygonDissolveProcessor(){}

	public:
		//virtual void	AddField(GField* pField) = 0;
	};

	/**
	 * 具有相同属性的多个多边形合并成一个
	 * 要执行一下distinct
	 */
	class PolygonDissolveByAttributeProcessor : public GProcessor
	{
	protected:
		PolygonDissolveByAttributeProcessor(){}
		virtual ~PolygonDissolveByAttributeProcessor(){}
	};



	/**
	 * 多边形形状指数、
	 * 1)面积	A
	 * 2)周长	P
	 * 3)周长/面积			P/A
	 * 4)周长/面积开平方	P/sqrt(A)
	 * 5)...参考qgis
	 */
	class PolygonShapeIndicesProcessor : public GProcessor
	{
	protected:
		PolygonShapeIndicesProcessor(){}
		virtual ~PolygonShapeIndicesProcessor(){}
	};

	/**
	 * 多边形属性
	 * 1)点个数
	 * 2)Part个数
	 * 3)周长
	 * 4)面积
	 */
	class PolygonPropertiesProcessor : public GProcessor
	{
	protected:
		PolygonPropertiesProcessor(){}
		virtual ~PolygonPropertiesProcessor(){}

	};

	//------------------------------------------------------------------------
	// Geometry Processor Begin
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	// Interpolation Processor Begin
	//------------------------------------------------------------------------
	class IDWProcessor : public GProcessor
	{
	protected:
		IDWProcessor(){}
		virtual ~IDWProcessor(){}
	public:
		virtual	void		SetInputDataSource(const char* sourceName) = 0;
		virtual	void		SetInputFeatureClass(const char* className) = 0;

		virtual	void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;

		virtual void		SetZField(const char* fname) = 0;
		virtual void		SetExtent(GEnvelope& extent) = 0;
		virtual void		SetCellSize(float cellSize) = 0;
	};

	class FeatureIDWProcessor : public GProcessor
	{
	protected:
		FeatureIDWProcessor(){}
		virtual ~FeatureIDWProcessor(){}
	public:
		virtual	void		SetInputDataSource(const char* sourceName) = 0;
		virtual	void		SetInputFeatureClass(const char* className) = 0;
		virtual void		SetInputZField(const char* fname) = 0;

		virtual	void		SetOutputDataSource(const char* sourceName) = 0;
		virtual	void		SetOutputFeatureClass(const char* className) = 0;
		virtual void		SetOutputZField(const char* fname) = 0;

		virtual void		SetExtent(GEnvelope& extent) = 0;
		virtual void		SetCellSize(double cellSize) = 0;
	};

	class DelaunayProcessor : public GProcessor
	{
	protected:
		DelaunayProcessor(){}
		virtual ~DelaunayProcessor(){}
	public:
		virtual	void		SetInputDataSource(const char* sourceName) = 0;
		virtual	void		SetInputFeatureClass(const char* className) = 0;
		virtual void		SetInputZField(const char* fname) = 0;

		virtual	void		SetOutputDataSource(const char* sourceName) = 0;
		virtual	void		SetOutputFeatureClass(const char* className) = 0;
	};

	//------------------------------------------------------------------------
	// Interpolation Processor End
	//------------------------------------------------------------------------

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

		virtual Raster*		Stretch(Raster* pinRaster) = 0;
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

	class RasterPixelBlendProcessor : public GProcessor
	{
	public:
		RasterPixelBlendProcessor(){}
		virtual ~RasterPixelBlendProcessor(){}
	public:
		virtual void		SetUser(g_uint user) = 0;

		virtual void		SetInputDataSource_1(const char* sourceName) = 0;
		virtual void		SetInputRaster_1(const char* rasterName) = 0;
		virtual void		SetInputPath_1(const char* rasterPath) = 0;

		virtual void		SetInputDataSource_2(const char* sourceName) = 0;
		virtual void		SetInputRaster_2(const char* rasterName) = 0;
		virtual void		SetInputPath_2(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;

		virtual RESULTCODE	Execute() = 0;
		virtual void		Release() = 0;
	};

	class RasterPenEffectProcessor : public GProcessor
	{
	protected:
		RasterPenEffectProcessor(){}
		virtual ~RasterPenEffectProcessor(){}
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

	class RasterHistogramEqualizationProcessor : public GProcessor
	{
	protected:
		RasterHistogramEqualizationProcessor(){}
		virtual ~RasterHistogramEqualizationProcessor(){}
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
	
	/**
	 * RasterThresholdProcessor
	 *
	 * 图像二值化
	 */
	class RasterThresholdProcessor : public GProcessor
	{
	public:
		RasterThresholdProcessor(){}
		virtual ~RasterThresholdProcessor(){}
	public:
		virtual void		SetUser(g_uint user) = 0;

		virtual void		SetInputDataSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;

		virtual void		SetThresholder(augeRasterThresholder smoother) = 0;

		virtual RESULTCODE	Execute() = 0;
		virtual void		Release() = 0;
	};

	class RasterClipProcessor  : public GProcessor
	{
	protected:
		RasterClipProcessor(){}
		virtual ~RasterClipProcessor(){}
	public:
		virtual void		SetUser(g_uint user) = 0;

		virtual void		SetInputRasterSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual	void		SetInputFeatureSource(const char* sourceName) = 0;
		virtual	void		SetInputFeatureClass(const char* className) = 0;
		virtual void		SetInputFilter(GFilter* pFilter) = 0;

		virtual void		SetOutpuRasteraSource(const char* sourceName) = 0;
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

	class RasterThumbnailProcessor : public GProcessor
	{
	protected:
		RasterThumbnailProcessor(){}
		virtual ~RasterThumbnailProcessor(){}
	public:
		virtual void		SetInputRaster(const char* rasterPath) = 0;
		virtual void		SetOutputRaster(const char* rasterPath) = 0;
		virtual RESULTCODE	Execute() = 0;
		virtual void		Release() = 0;
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

		virtual Raster*		Slope(Raster* pinRaster) = 0;
	};

	/**
	 * DemAspectProcessor
	 *
	 * DEM坡向
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

		virtual Raster*		Aspect(Raster* pinRaster) = 0;

	};

	/**
	 * DemHillshadeProcessor
	 *
	 * DEM山体阴影
	 */
	class DemHillshadeProcessor : public GProcessor
	{
	protected:
		DemHillshadeProcessor(){}
		virtual ~DemHillshadeProcessor(){}
	public:
		virtual void		SetInputDataSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;

		virtual void		SetAzimuth(float azimuth) = 0;		//太阳方位角
		virtual void		SetZenith(float zenith) = 0;		//太阳高度角
		virtual void		SetZFactor(float zscale) = 0;		//Z比例因子
	};

	/**
	 * DemReliefAmplitue
	 *
	 * 地形起伏度
	 */
	class DemReliefAmplitue : public GProcessor
	{
	protected:
		DemReliefAmplitue(){}
		virtual ~DemReliefAmplitue(){}
	public:
		virtual void		SetInputDataSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;
	};

	/**
	 * DemEarthSurfaceIncision
	 *
	 * 地表切割深度
	 */
	class DemEarthSurfaceIncision : public GProcessor
	{
	protected:
		DemEarthSurfaceIncision(){}
		virtual ~DemEarthSurfaceIncision(){}
	public:
		virtual void		SetInputDataSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;
	};

	/**
	 * DemRoughness
	 *
	 * 地表粗糙度
	 */
	class DemRoughness : public GProcessor
	{
	protected:
		DemRoughness(){}
		virtual ~DemRoughness(){}
	public:
		virtual void		SetInputDataSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;
	};

	/**
	 * 	DemVarainceCofficientniEelvation
	 *
	 * 地形高程变异系数
	 */
	class DemVarainceCofficientniEelvation : public GProcessor
	{
	protected:
		DemVarainceCofficientniEelvation(){}
		virtual ~DemVarainceCofficientniEelvation(){}
	public:
		virtual void		SetInputDataSource(const char* sourceName) = 0;
		virtual void		SetInputRaster(const char* rasterName) = 0;
		virtual void		SetInputPath(const char* rasterPath) = 0;

		virtual void		SetOutputDataSource(const char* sourceName) = 0;
		virtual void		SetOutputRaster(const char* rasterName) = 0;
		virtual void		SetOutputPath(const char* rasterPath) = 0;
	};

	//------------------------------------------------------------------------
	// Raster Processor End
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	// Grid Processor Begin
	//------------------------------------------------------------------------
	class GridPointGenerator : public GProcessor
	{
	protected:
		GridPointGenerator(){}
		virtual ~GridPointGenerator(){}
	public:
		virtual	void		SetOutputDataSource(const char* sourceName) = 0;
		virtual	void		SetOutputFeatureClass(const char* className) = 0;
		virtual void		SetOutputSRID(g_uint srid) = 0;

		virtual void		SetExtent(GEnvelope& extent) = 0;
		virtual void		SetCellSize(float cellSize) = 0;
	};
	//------------------------------------------------------------------------
	// Grid Processor Begin
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

	class DBScanProcessor : public GProcessor
	{
	protected:
		DBScanProcessor(){}
		virtual ~DBScanProcessor(){}
	public:
		virtual void		SetEsplon(float esplon) = 0;
		virtual void		SetMinPoints(g_uint minpts) = 0;
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
		virtual FeatureExportProcessor*		CreateFeatureExportProcessor() = 0;
		virtual RandomPointsGenerator*		CreateRandomPointsGenerator() = 0;
		virtual RandomPointsInPolygonGenerator* CreateRandomPointsInPolygonGenerator() = 0;

		virtual CsvImportProcessor*			CreateCsvImportProcessor() = 0;
		virtual XYImportProcessor*			CreateXYImportProcessor() = 0;

		// Geometry
		virtual MultiPointToPointsProcessor* CreateMultiPointToPointsProcessor() = 0;
		virtual LineToPointsProcessor*		CreateLineToPointsProcessor() = 0;
		virtual PolygonToPointsProcessor*	CreatePolygonToPointsProcessor() = 0;
		virtual PolygonToLineProcessor*		CreatePolygonToLineProcessor() = 0;

		// Cluster 
		virtual KMeansProcessor*			CreateKMeansProcessor() = 0;
		virtual DBScanProcessor*			CreateDBScanProcessor() = 0;

		// Grid
		virtual GridPointGenerator*			CreateGridPointGenerator() = 0;

		// projection
		virtual FeatureProjectProcessor*	CreateFeatureProjectProcessor() = 0;

		// Inteplate
		virtual FeatureIDWProcessor*		CreateFeatureIDWProcessor() = 0;
		virtual DelaunayProcessor*			CreateDelaunayProcessor() = 0;

		// Raster
		virtual RasterGraylizeProcessor*			CreateRasterGraylizeProcessor() = 0;
		virtual RasterReverseProcessor*				CreateRasterReverseProcessor() = 0;
		virtual RasterThumbnailProcessor*			CreateRasterThumbnailProcessor() = 0;

		virtual RasterExtractByRectangleProcessor*	CreateRasterExtractByRectangleProcessor() = 0;

		// 图形增强
		virtual RasterStretchProcessor*				CreateRasterStretchProcessor() = 0;
		virtual RasterEdgeDetectProcessor*			CreateRasterEdgeDetectProcessor() = 0;		
		virtual RasterSmoothProcessor*				CreateRasterSmoothProcessor() = 0;
		virtual RasterSubtractProcessor*			CreateRasterSubtractProcessor() = 0;
		virtual RasterHistogramEqualizationProcessor* CreateRasterHistogramEqualizationProcessor() = 0;

		// 图像融合
		virtual RasterPixelBlendProcessor*			CreateRasterPixelBlendProcessor() = 0;
		// 图像分割
		virtual RasterThresholdProcessor*			CreateRasterThresholdProcessor() = 0;

		virtual RasterFormatConvertToJPEGProcessor*	CreateRasterFormatConvertToJPEGProcessor() = 0;
		// 图像特效
		virtual RasterSepiaToneEffectProcessor*		CreateRasterSepiaToneEffectProcessor() = 0;
		virtual RasterPenEffectProcessor*			CreateRasterPenEffectProcessor() = 0;

		// DEM
		virtual DemSlopeProcessor*					CreateDemSlopeProcessor() = 0;
		virtual DemAspectProcessor*					CreateDemAspectProcessor() = 0;
		virtual DemHillshadeProcessor*				CreateDemHillshadeProcessor() = 0;
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
