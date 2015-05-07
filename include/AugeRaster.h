#ifndef __AUGE_RASTER_H__
#define __AUGE_RASTER_H__

#include "AugeData.h"
#include "AugeGeometry.h"

#ifdef WIN32
#	ifdef AUGE_RASTER_EXPORTS
#		define AUGE_EATURE_API __declspec(dllexport)
#	else
#		define AUGE_EATURE_API __declspec(dllimport)
#	endif
#else
#	define AUGE_EATURE_API
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

	class RasterWorkspace : public Workspace
	{
	protected:
		RasterWorkspace(){}
		virtual ~RasterWorkspace(){}
	public:
		virtual	Raster*			OpenRaster(const char* name) = 0;
	};

	class Raster : public DataSet
	{
	protected:
		Raster(){}
		virtual ~Raster(){}
	public:
		virtual g_uint			GetWidth() = 0;
		virtual g_uint			GetHeight() = 0;
		virtual g_uint			GetBandCount() = 0;
		virtual augePixelType	GetPixelType() = 0;
		virtual GEnvelope&		GetExtent() = 0;
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
		virtual augePixelType	GetPixelType() = 0;
	};
}

#endif //__AUGE_RASTER_H__
