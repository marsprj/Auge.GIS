#include "RasterIOImpl.h"
#include "RasterImpl.h"

namespace auge
{
	RasterIO* augeGetRasterIOInstance()
	{
		static RasterIOImpl g_rasterIO;
		return &g_rasterIO;
	}

	RasterIOImpl::RasterIOImpl()
	{
		GDALAllRegister();
	}

	RasterIOImpl::~RasterIOImpl()
	{

	}

	Raster*	RasterIOImpl::Read(const char* path)
	{
		if(path==NULL)
		{
			return NULL;
		}

		if(g_access(path,4))
		{
			return NULL;
		}

		g_char drv[AUGE_NAME_MAX];
		g_char dir[AUGE_NAME_MAX];
		g_char name[AUGE_NAME_MAX];
		g_char ext[AUGE_EXT_MAX];
		g_char full_name[AUGE_NAME_MAX];

		auge_split_path(path, drv, dir, name, ext);
		auge_make_path(full_name, NULL, NULL, name, ext);
		
		RasterImpl* pRaster = new RasterImpl();
		pRaster->Create(full_name, path);
		return pRaster;
	}

	Raster*	RasterIOImpl::Read(const char* path, const char* name)
	{
		if((path==NULL)||(name==NULL))
		{
			return NULL;
		}

		char raster_path[AUGE_PATH_MAX];
		auge_make_path(raster_path, NULL, path, name, NULL);

		if(!g_access(path,4))
		{
			return NULL;
		}

		RasterImpl* pRaster = new RasterImpl();
		pRaster->Create(name, raster_path);
		return pRaster;
	}
}