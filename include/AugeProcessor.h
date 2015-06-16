#ifndef __AUGE_GEO_PROCESSOR_H__
#define __AUGE_GEO_PROCESSOR_H__

#include "AugeType.h"
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

	class GProcessor : public GObject
	{
	protected:
		GProcessor(){}
		virtual ~GProcessor(){}
	public:
		virtual RESULTCODE	Execute() = 0;
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
