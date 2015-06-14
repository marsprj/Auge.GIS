#ifndef __AUGE_RASTER_H__
#define __AUGE_RASTER_H__

#include "AugeCore.h"
#include "AugeGeometry.h"

#ifdef WIN32
#	ifdef AUGE_RASTER_EXPORTS
#		define AUGE_RASTER_API __declspec(dllexport)
#	else
#		define AUGE_RASTER_API __declspec(dllimport)
#	endif
#else
#	define AUGE_RASTER_API
#endif

namespace auge
{
	class Raster;
	class RasterBand;
	class RasterWorkspace;

	typedef enum
	{
		augePixelUnknown	= 0,
		augePixelByte		= 1,
		augePixelUInt16		= 2,
		augePixelInt16		= 3,
		augePixelUInt32		= 4,
		augePixelInt32		= 5,
		augePixelFloat32	= 6,
		augePixelDouble		= 7
	}augePixelType;

	class Raster : public GObject
	{
	protected:
		Raster(){}
		virtual ~Raster(){}
	public:
		virtual const char*		GetName() = 0;
		virtual const char*		GetAlias() = 0;
		//virtual augeDataSetType	GetType() = 0;
		virtual g_uint			GetWidth() = 0;
		virtual g_uint			GetHeight() = 0;
		virtual g_uint			GetBandCount() = 0;
		virtual RasterBand*		GetBand(g_uint i) = 0;
		virtual GEnvelope&		GetExtent() = 0;
		virtual g_int			GetSRID() = 0;
		virtual const char*		GetFormat() = 0;

		virtual augePixelType	GetPixelType() = 0;
		virtual g_uint			GetPixelSize() = 0;

		virtual bool			GetMapPosition(g_uint rx, g_uint ry, double& mx, double& my) = 0;
		virtual bool			GetRasterPosition(double mx, double my, g_uint& rx, g_uint& ry) = 0;

		virtual const char*		GetPath() = 0;
	};

	class RasterBand : public GObject
	{
	protected:
		RasterBand(){}
		virtual ~RasterBand(){}
	public:
		virtual g_uint			GetWidth() = 0;
		virtual g_uint			GetHeight() = 0;
		virtual GEnvelope&		GetExtent() = 0;

		virtual augePixelType	GetPixelType() = 0;
		virtual g_uint			GetPixelSize() = 0;

		virtual g_uint			GetDataSize() = 0;
		virtual void*			GetData() = 0;

		virtual void*			GetData(int x, int y) = 0;
		virtual void*			GetData(double x, double y) = 0;
	};

	class RasterIO
	{
	protected:
		RasterIO(){}
		virtual ~RasterIO(){}
	public:
		virtual Raster*			Read(const char* path)= 0;
		virtual Raster*			Read(const char* path, const char* name)= 0;
	};

	extern "C"
	{
		AUGE_RASTER_API RasterIO*	augeGetRasterIOInstance();
	}
}

//==============================================================================
//------------------------------------------------------------------------------
// �������趯̬��
//------------------------------------------------------------------------------
#ifdef WIN32
#	ifndef AUGE_RASTER_EXPORTS
#		define _AUGE_RASTER_NAME "GRaster"
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
#		pragma comment(lib, _AUGE_RASTER_NAME _AUGE_FILTER_CONFIGURATION ".lib")
#		pragma message("Automatically linking with " _AUGE_RASTER_NAME _AUGE_FILTER_CONFIGURATION ".dll")
#		undef _AUGE_FILTER_CONFIGURATION
#		undef _AUGE_RASTER_NAME
#	endif // AUGE_FILTER_EXPORTS 
#endif
//==============================================================================


#endif //__AUGE_RASTER_H__
