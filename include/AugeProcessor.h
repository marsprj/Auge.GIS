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

	class GProcessor : public GObject
	{
	protected:
		GProcessor(){}
		virtual ~GProcessor(){}
	public:
		virtual RESULTCODE	Execute() = 0;
	};

	class TileStoreGenerator : public GProcessor
	{
	protected:
		TileStoreGenerator(){}
		virtual ~TileStoreGenerator(){}
	public:
		virtual	void	SetMap(Map* pMap) = 0;
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

		virtual RESULTCODE	Compress(const char* tpath) = 0;
		virtual RESULTCODE	Decompress(const char* tpath) = 0;

		virtual void		AddFile(const char* fpath) = 0;
		virtual void		Clear() = 0;

		virtual void		Release() = 0;
	};

	class GProcessorFactory
	{
	protected:
		GProcessorFactory(){}
		virtual ~GProcessorFactory(){}
	public:
		virtual TileStoreGenerator*	CreateTileStoreGenerator() = 0;
		virtual ArchiveProcessor*	CreateArchiveProcessor() = 0;
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
