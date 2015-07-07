#include "RasterFactoryImpl.h"
#include "RasterImpl.h"

#include <gdal/gdal_priv.h>

namespace auge
{
	RasterFactory* augeGetRasterFactoryInstance()
	{
		static RasterFactoryImpl g_rasterFactory;
		return &g_rasterFactory;
	}

	RasterFactoryImpl::RasterFactoryImpl()
	{

	}

	RasterFactoryImpl::~RasterFactoryImpl()
	{

	}

	//创建带spatialreference的图像时有bug
	Raster*	RasterFactoryImpl::CreateRaster(const char* name, GEnvelope& extent, Raster* pinRaster)
	{
		if(pinRaster==NULL)
		{
			return NULL;
		}

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		GRect rect;
		if(!pinRaster->GetRasterRect(rect,extent))
		{
			return NULL;
		}
		g_uint r_width = rect.GetWidth();
		g_uint r_height= rect.GetHeight();
		GDALDataType pixelType =  (GDALDataType)(pinRaster->GetPixelType());
		const char*  spatialref = pinRaster->GetSpatialReference();

		// Create Dataset in memory
		GDALDriver* pmemDriver  = NULL;
		pmemDriver = GetGDALDriverManager()->GetDriverByName("MEM");
		GDALDataset* pmemDataset = NULL;
		pmemDataset = pmemDriver->Create("", r_width, r_height, pinRaster->GetBandCount(), pixelType, NULL);
		if(pmemDataset==NULL)
		{
			const char* msg = CPLGetLastErrorMsg();			
			pError->SetError(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return NULL;
		}
		// GeoTransform
		double adfGeoTransform[6];
		adfGeoTransform[0] = extent.m_xmin;							/* top left x */
		adfGeoTransform[1] = extent.GetWidth() / rect.GetWidth();	/* w-e pixel resolution */
		adfGeoTransform[2] = 0;										/* 0 */
		adfGeoTransform[3] = extent.m_ymax;							/* top left y */
		adfGeoTransform[4] = 0;										/* 0 */
		adfGeoTransform[5] =-extent.GetHeight() / rect.GetHeight();	/* n-s pixel resolution (negative value) */
		pmemDataset->SetGeoTransform(adfGeoTransform);
		// spatial reference
		pmemDataset->SetProjection(spatialref);

		// read and write new raster
		g_uint64 buffer_size = (g_uint64)r_width * (g_uint64)r_height * pinRaster->GetPixelSize();
		unsigned char* buffer = (unsigned char*)malloc(buffer_size);

		CPLErr err = CE_None;
		RasterBand* pBand = NULL;
		GDALRasterBand* pmemBand = NULL;
		g_uint band_count = pinRaster->GetBandCount();
		for(g_uint i=0; i<band_count; i++)
		{
			pBand = pinRaster->GetBand(i);
			pmemBand = pmemDataset->GetRasterBand(i+1);

			memset(buffer, 0, buffer_size);
			pBand->Read(buffer, rect.m_xmin, rect.m_ymin, r_width, r_height);

			CPLErr err = pmemBand->RasterIO(GF_Write, 
											0, 0,
											r_width, r_height, 
											buffer,
											r_width, r_height,
											pixelType, 
											0, 0 );
			if(err>CE_Warning)
			{
				break;
			}
		}

		free(buffer);

		if(err>CE_Warning)
		{
			GDALClose(pmemDataset);
			return NULL;
		}

		RasterImpl* poutRaster = new RasterImpl();
		poutRaster->Create(name, pmemDataset);
		return poutRaster;
	}
	
	Raster*	RasterFactoryImpl::CreateRasterPNG(const char* name, g_uint width, g_uint height)
	{
		if(name==NULL)
		{
			return NULL;
		}

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		g_uint band_count = 4;
		GDALDataType pixelType = GDT_Byte;
		GDALDriver* pmemDriver  = NULL;
		pmemDriver = GetGDALDriverManager()->GetDriverByName("MEM");

		GDALDataset* pmemDataset = NULL;
		pmemDataset = pmemDriver->Create(name, width, height, 4, pixelType, NULL);
		if(pmemDataset==NULL)
		{
			const char* msg = CPLGetLastErrorMsg();			
			pError->SetError(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return NULL;
		}

		//// GeoTransform
		//double adfGeoTransform[6];
		//adfGeoTransform[0] = 0;							/* top left x */
		//adfGeoTransform[1] = 1;							/* w-e pixel resolution */
		//adfGeoTransform[2] = 0;							/* 0 */
		//adfGeoTransform[3] = height;					/* top left y */
		//adfGeoTransform[4] = 0;							/* 0 */
		//adfGeoTransform[5] =-1;							/* n-s pixel resolution (negative value) */
		//pmemDataset->SetGeoTransform(adfGeoTransform);
		//// spatial reference
		//pmemDataset->SetProjection("");

		// read and write new raster
		g_uint64 buffer_size = (g_uint64)width * (g_uint64)height * sizeof(char);
		unsigned char* buffer = (unsigned char*)malloc(buffer_size);
		memset(buffer, 0, buffer_size);

		CPLErr err = CE_None;
		RasterBand* pBand = NULL;
		GDALRasterBand* pmemBand = NULL;
		for(g_uint i=0; i<band_count; i++)
		{
			memset(buffer, 0, buffer_size);
			pmemBand = pmemDataset->GetRasterBand(i+1);
			if(pmemBand==NULL)
			{
				break;
			}
			CPLErr err = pmemBand->RasterIO(GF_Write, 
											0, 0,
											width, height, 
											buffer,
											width, height,
											pixelType, 
											0, 0 );
			if(err>CE_Warning)
			{
				break;
			}
		}

		free(buffer);

		if(err>CE_Warning)
		{
			GDALClose(pmemDataset);
			return NULL;
		}

		RasterImpl* poutRaster = new RasterImpl();
		poutRaster->Create(name, pmemDataset);
		return poutRaster;
	}

	Raster*	RasterFactoryImpl::CreateRasterJPG(const char* name, g_uint width, g_uint height)
	{
		if(name==NULL)
		{
			return NULL;
		}

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		g_uint band_count = 3;
		GDALDataType pixelType = GDT_Byte;
		GDALDriver* pmemDriver  = NULL;
		pmemDriver = GetGDALDriverManager()->GetDriverByName("MEM");

		GDALDataset* pmemDataset = NULL;
		pmemDataset = pmemDriver->Create(name, width, height, 4, pixelType, NULL);
		if(pmemDataset==NULL)
		{
			const char* msg = CPLGetLastErrorMsg();			
			pError->SetError(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return NULL;
		}

		//// GeoTransform
		//double adfGeoTransform[6];
		//adfGeoTransform[0] = 0;							/* top left x */
		//adfGeoTransform[1] = 1;							/* w-e pixel resolution */
		//adfGeoTransform[2] = 0;							/* 0 */
		//adfGeoTransform[3] = height;					/* top left y */
		//adfGeoTransform[4] = 0;							/* 0 */
		//adfGeoTransform[5] =-1;							/* n-s pixel resolution (negative value) */
		//pmemDataset->SetGeoTransform(adfGeoTransform);
		//// spatial reference
		//pmemDataset->SetProjection("");

		// read and write new raster
		g_uint64 buffer_size = (g_uint64)width * (g_uint64)height * sizeof(char);
		unsigned char* buffer = (unsigned char*)malloc(buffer_size);
		memset(buffer, 0, buffer_size);

		CPLErr err = CE_None;
		RasterBand* pBand = NULL;
		GDALRasterBand* pmemBand = NULL;
		for(g_uint i=0; i<band_count; i++)
		{
			memset(buffer, 0, buffer_size);
			pmemBand = pmemDataset->GetRasterBand(i+1);
			if(pmemBand==NULL)
			{
				break;
			}
			CPLErr err = pmemBand->RasterIO(GF_Write, 
				0, 0,
				width, height, 
				buffer,
				width, height,
				pixelType, 
				0, 0 );
			if(err>CE_Warning)
			{
				break;
			}
		}

		free(buffer);

		if(err>CE_Warning)
		{
			GDALClose(pmemDataset);
			return NULL;
		}

		RasterImpl* poutRaster = new RasterImpl();
		poutRaster->Create(name, pmemDataset);
		return poutRaster;
	}

	Raster* RasterFactoryImpl::CreateRaster(const char* name, augePixelType pixelType, g_uint width, g_uint height, g_int bands, g_uint srid, GEnvelope& extent)
	{
		return NULL;
	}
}