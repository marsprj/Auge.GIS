#include "RasterProjectProcessorImpl.h"

#include "AugeData.h"
#include "AugeCore.h"
#include "AugeRaster.h"
#include "AugeUser.h"

#include "proj_api.h"
#include "SrsBase.h"

#include <gdal/gdalwarper.h>
#include <gdal/cpl_conv.h>
#include <gdal/ogr_spatialref.h>

namespace auge
{
	void make_raster_path(char* rpath, g_uint rsize, const char* repository,  const char* raster_path, const char* raster_name);

	RasterProjectProcessorImpl::RasterProjectProcessorImpl()
	{
		m_user = 0;
		m_in_raster_path = "/";
		m_out_raster_path = "/";
		m_out_srid = AUGE_DEFAULT_SRID;
	}

	RasterProjectProcessorImpl::~RasterProjectProcessorImpl()
	{

	}

	void RasterProjectProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}

	void RasterProjectProcessorImpl::SetInputDataSource(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_in_source_name.clear();
		}
		else
		{
			m_in_source_name = sourceName;
		}
	}

	void RasterProjectProcessorImpl::SetInputRaster(const char* rasterName)
	{
		if(rasterName==NULL)
		{
			m_in_raster_name.clear();
		}
		else
		{
			m_in_raster_name = rasterName;
		}
	}

	void RasterProjectProcessorImpl::SetOutputDataSource(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_out_source_name.clear();
		}
		else
		{
			m_out_source_name = sourceName;
		}
	}

	void RasterProjectProcessorImpl::SetOutputRaster(const char* rasterName)
	{
		if(rasterName==NULL)
		{
			m_out_raster_name.clear();
		}
		else
		{
			m_out_raster_name = rasterName;
		}
	}

	const char*	RasterProjectProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	RasterProjectProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	RasterProjectProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterProjectProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	void RasterProjectProcessorImpl::SetInputPath(const char* rasterPath)
	{
		if(rasterPath==NULL)
		{
			m_in_raster_path.clear();
		}
		else
		{
			m_in_raster_path = rasterPath;
		}
	}

	void RasterProjectProcessorImpl::SetOutputPath(const char* rasterPath)
	{
		if(rasterPath==NULL)
		{
			m_out_raster_path.clear();
		}
		else
		{
			m_out_raster_path = rasterPath;
		}
	}

	const char* RasterProjectProcessorImpl::GetInputRasterPath()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char* RasterProjectProcessorImpl::GetOutputRasterPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	void RasterProjectProcessorImpl::SetOutputSrid(g_uint srid)
	{
		m_out_srid = srid;
	}

	g_uint RasterProjectProcessorImpl::GetOutputSrid()
	{
		return m_out_srid;
	}

	RESULTCODE RasterProjectProcessorImpl::Execute()
	{
		const char* inSourceName = GetInputDataSource();
		const char* inRasterName = GetInputRaster();
		const char* inRasterPath = GetInputRasterPath();
		const char* outSourceName = GetOutputDataSource();
		const char* outRasterName = GetOutputRaster();
		const char* outRasterPath = GetOutputRasterPath();

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		Workspace* pWorkspace = NULL;
		RasterWorkspace* pinRasterWorkspace = NULL;
		RasterWorkspace* poutRasterWorkspace = NULL;

		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = pConnManager->GetWorkspace(m_user, inSourceName);
		if(pWorkspace==NULL)
		{
			return AG_FAILURE;
		}
		pinRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

		pWorkspace = pConnManager->GetWorkspace(m_user, outSourceName);
		if(pWorkspace==NULL)
		{
			return AG_FAILURE;
		}
		poutRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

		char inpath[AUGE_PATH_MAX];
		char outpath[AUGE_PATH_MAX];
		make_raster_path( inpath, AUGE_PATH_MAX,  pinRasterWorkspace->GetRepository(),  inRasterPath,  inRasterName);
		make_raster_path(outpath, AUGE_PATH_MAX, poutRasterWorkspace->GetRepository(), outRasterPath, outRasterName);

		
		return Project(inpath, outpath, m_out_srid);
	}

	void RasterProjectProcessorImpl::Release()
	{
		delete this;
	}

	Raster*	RasterProjectProcessorImpl::Project(Raster* pRaster, g_uint srid)
	{
		const char* opath = "h:\\temp\\result\\900913.tif";

		return NULL;
	}

	RESULTCODE RasterProjectProcessorImpl::Project(const char* inpath, const char* outpath, g_uint out_sird)
	{
		GDALDriverH hDriver;
		GDALDataType eDT;
		GDALDatasetH hDstDS;
		GDALDatasetH hSrcDS;
		RESULTCODE rc = AG_SUCCESS;

		GError*	 pError  = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		GDALAllRegister();
		OGRRegisterAll();

		SrsBase	m_srsbase;
		rc = m_srsbase.Open();
		if(rc!=AG_SUCCESS)
		{
			const char* msg = "";
			return rc;
		}

		char out_srs[AUGE_MSG_MAX];
		memset(out_srs, 0, AUGE_MSG_MAX);
		rc = m_srsbase.GetProj4Text(out_sird, out_srs, AUGE_MSG_MAX);
		if(rc != AG_SUCCESS)
		{
			m_srsbase.Close();
			return AG_SUCCESS;
		}


		{
			// Open the source file. 
			hSrcDS = GDALOpen( inpath, GA_ReadOnly );
			CPLAssert( hSrcDS != NULL );

			// Create output with same datatype as first input band. 
			eDT = GDALGetRasterDataType(GDALGetRasterBand(hSrcDS,1));
			// Get output driver (GeoTIFF format)
			hDriver = GDALGetDriverByName( "GTiff" );
			CPLAssert( hDriver != NULL );
			// Get Source coordinate system. 
			const char *pszSrcWKT= NULL;
			char *pszDstWKT = NULL;
			pszSrcWKT = GDALGetProjectionRef( hSrcDS );
			CPLAssert( pszSrcWKT != NULL && strlen(pszSrcWKT) > 0 );
			// Setup output coordinate system that is UTM 11 WGS84. 
			OGRSpatialReference oSRS;
			OGRErr err = oSRS.importFromProj4(out_srs);
			//oSRS.SetUTM( 11, TRUE );
			//oSRS.SetWellKnownGeogCS( "WGS84" );
			oSRS.exportToWkt( &pszDstWKT );
			// Create a transformer that maps from source pixel/line coordinates
			// to destination georeferenced coordinates (not destination 
			// pixel line).  We do that by omitting the destination dataset
			// handle (setting it to NULL). 


			void *hTransformArg;
			hTransformArg = 
				GDALCreateGenImgProjTransformer( hSrcDS, pszSrcWKT, NULL, pszDstWKT, 
				FALSE, 0, 1 );
			CPLAssert( hTransformArg != NULL );
			// Get approximate output georeferenced bounds and resolution for file. 
			double adfDstGeoTransform[6];
			int nPixels=0, nLines=0;
			CPLErr eErr;
			eErr = GDALSuggestedWarpOutput( hSrcDS, 
				GDALGenImgProjTransform, hTransformArg, 
				adfDstGeoTransform, &nPixels, &nLines );

			CPLAssert( eErr == CE_None );
			GDALDestroyGenImgProjTransformer( hTransformArg );
			// Create the output file.
			int bandCount = GDALGetRasterCount(hSrcDS);
			hDstDS = GDALCreate( hDriver, outpath, nPixels, nLines, 
				GDALGetRasterCount(hSrcDS), eDT, NULL );

			CPLAssert( hDstDS != NULL );
			// Write out the projection definition. 
			GDALSetProjection( hDstDS, pszDstWKT );
			GDALSetGeoTransform( hDstDS, adfDstGeoTransform );
			// Copy the color table, if required.
			GDALColorTableH hCT;
			hCT = GDALGetRasterColorTable( GDALGetRasterBand(hSrcDS,1) );
			if( hCT != NULL )
				GDALSetRasterColorTable( GDALGetRasterBand(hDstDS,1), hCT );
		}

		{
			GDALWarpOptions *psWarpOptions = GDALCreateWarpOptions();
			psWarpOptions->hSrcDS = hSrcDS;
			psWarpOptions->hDstDS = hDstDS;
			//psWarpOptions->nBandCount = 1;
			//psWarpOptions->panSrcBands = 
			//	(int *) CPLMalloc(sizeof(int) * psWarpOptions->nBandCount );
			//psWarpOptions->panSrcBands[0] = 3;
			//psWarpOptions->panDstBands = 
			//	(int *) CPLMalloc(sizeof(int) * psWarpOptions->nBandCount );
			//psWarpOptions->panDstBands[0] = 3;

			psWarpOptions->nBandCount = GDALGetRasterCount(hSrcDS);;
			psWarpOptions->panSrcBands = 
				(int *) CPLMalloc(sizeof(int) * psWarpOptions->nBandCount );
			psWarpOptions->panDstBands = 
				(int *) CPLMalloc(sizeof(int) * psWarpOptions->nBandCount );

			for(int i=0; i<psWarpOptions->nBandCount; i++)
			{
				psWarpOptions->panSrcBands[i] = i+1;
				psWarpOptions->panDstBands[i] = i+1;
			}


			psWarpOptions->pfnProgress = GDALTermProgress;   
			// Establish reprojection transformer. 
			psWarpOptions->pTransformerArg = 
				GDALCreateGenImgProjTransformer( hSrcDS, 
				GDALGetProjectionRef(hSrcDS), 
				hDstDS,
				GDALGetProjectionRef(hDstDS), 
				FALSE, 0.0, 1 );
			psWarpOptions->pfnTransformer = GDALGenImgProjTransform;
			// Initialize and execute the warp operation. 
			GDALWarpOperation oOperation;
			oOperation.Initialize( psWarpOptions );

			oOperation.ChunkAndWarpImage( 0, 0, 
				GDALGetRasterXSize( hDstDS ), 
				GDALGetRasterYSize( hDstDS ) );

			GDALDestroyGenImgProjTransformer( psWarpOptions->pTransformerArg );
			GDALDestroyWarpOptions( psWarpOptions );
			GDALClose( hDstDS );
			GDALClose( hSrcDS );
		}

		m_srsbase.Close();
	}

	//RESULTCODE RasterProjectProcessorImpl::Project(const char* inpath, const char* outpath, g_uint out_sird)
	//{
	//	GDALDriverH hDriver;
	//	GDALDataType eDT;
	//	GDALDatasetH hDstDS;
	//	GDALDatasetH hSrcDS;

	//	GDALAllRegister();
	//	OGRRegisterAll();
	//	
	//	RESULTCODE rc = AG_SUCCESS;
	//	
	//	SrsBase	m_srsbase;
	//	rc = m_srsbase.Open();
	//	if(rc!=AG_SUCCESS)
	//	{
	//		return rc;
	//	}

	//	char out_srs[AUGE_MSG_MAX];
	//	memset(out_srs, 0, AUGE_MSG_MAX);
	//	rc = m_srsbase.GetProj4Text(out_sird, out_srs, AUGE_MSG_MAX);
	//	if(rc != AG_SUCCESS)
	//	{
	//		m_srsbase.Close();
	//		return AG_SUCCESS;
	//	}


	//	{
	//		// Open the source file. 
	//		hSrcDS = GDALOpen( inpath, GA_ReadOnly );
	//		CPLAssert( hSrcDS != NULL );

	//		// Create output with same datatype as first input band. 
	//		eDT = GDALGetRasterDataType(GDALGetRasterBand(hSrcDS,1));
	//		// Get output driver (GeoTIFF format)
	//		hDriver = GDALGetDriverByName( "GTiff" );
	//		CPLAssert( hDriver != NULL );
	//		// Get Source coordinate system. 
	//		const char *pszSrcWKT= NULL;
	//		char *pszDstWKT = NULL;
	//		pszSrcWKT = GDALGetProjectionRef( hSrcDS );
	//		CPLAssert( pszSrcWKT != NULL && strlen(pszSrcWKT) > 0 );
	//		// Setup output coordinate system that is UTM 11 WGS84. 
	//		OGRSpatialReference oSRS;
	//		OGRErr err = oSRS.importFromProj4(out_srs);
	//		//oSRS.SetUTM( 11, TRUE );
	//		//oSRS.SetWellKnownGeogCS( "WGS84" );
	//		oSRS.exportToWkt( &pszDstWKT );
	//		// Create a transformer that maps from source pixel/line coordinates
	//		// to destination georeferenced coordinates (not destination 
	//		// pixel line).  We do that by omitting the destination dataset
	//		// handle (setting it to NULL). 
	//		void *hTransformArg;
	//		hTransformArg = 
	//			GDALCreateGenImgProjTransformer( hSrcDS, pszSrcWKT, NULL, pszDstWKT, 
	//			FALSE, 0, 1 );
	//		CPLAssert( hTransformArg != NULL );
	//		// Get approximate output georeferenced bounds and resolution for file. 
	//		double adfDstGeoTransform[6];
	//		int nPixels=0, nLines=0;
	//		CPLErr eErr;
	//		eErr = GDALSuggestedWarpOutput( hSrcDS, 
	//			GDALGenImgProjTransform, hTransformArg, 
	//			adfDstGeoTransform, &nPixels, &nLines );
	//		CPLAssert( eErr == CE_None );
	//		GDALDestroyGenImgProjTransformer( hTransformArg );
	//		// Create the output file.
	//		int bandCount = GDALGetRasterCount(hSrcDS);
	//		hDstDS = GDALCreate( hDriver, outpath, nPixels, nLines, 
	//			GDALGetRasterCount(hSrcDS), eDT, NULL );

	//		CPLAssert( hDstDS != NULL );
	//		// Write out the projection definition. 
	//		GDALSetProjection( hDstDS, pszDstWKT );
	//		GDALSetGeoTransform( hDstDS, adfDstGeoTransform );
	//		// Copy the color table, if required.
	//		GDALColorTableH hCT;
	//		hCT = GDALGetRasterColorTable( GDALGetRasterBand(hSrcDS,1) );
	//		if( hCT != NULL )
	//			GDALSetRasterColorTable( GDALGetRasterBand(hDstDS,1), hCT );
	//	}

	//	{
	//		GDALWarpOptions *psWarpOptions = GDALCreateWarpOptions();
	//		psWarpOptions->hSrcDS = hSrcDS;
	//		psWarpOptions->hDstDS = hDstDS;
	//		//psWarpOptions->nBandCount = 1;
	//		//psWarpOptions->panSrcBands = 
	//		//	(int *) CPLMalloc(sizeof(int) * psWarpOptions->nBandCount );
	//		//psWarpOptions->panSrcBands[0] = 3;
	//		//psWarpOptions->panDstBands = 
	//		//	(int *) CPLMalloc(sizeof(int) * psWarpOptions->nBandCount );
	//		//psWarpOptions->panDstBands[0] = 3;

	//		psWarpOptions->nBandCount = GDALGetRasterCount(hSrcDS);;
	//		psWarpOptions->panSrcBands = 
	//			(int *) CPLMalloc(sizeof(int) * psWarpOptions->nBandCount );
	//		psWarpOptions->panDstBands = 
	//			(int *) CPLMalloc(sizeof(int) * psWarpOptions->nBandCount );

	//		for(int i=0; i<psWarpOptions->nBandCount; i++)
	//		{
	//			psWarpOptions->panSrcBands[i] = i+1;
	//			psWarpOptions->panDstBands[i] = i+1;
	//		}


	//		psWarpOptions->pfnProgress = GDALTermProgress;   
	//		// Establish reprojection transformer. 
	//		psWarpOptions->pTransformerArg = 
	//			GDALCreateGenImgProjTransformer( hSrcDS, 
	//			GDALGetProjectionRef(hSrcDS), 
	//			hDstDS,
	//			GDALGetProjectionRef(hDstDS), 
	//			FALSE, 0.0, 1 );
	//		psWarpOptions->pfnTransformer = GDALGenImgProjTransform;
	//		// Initialize and execute the warp operation. 
	//		GDALWarpOperation oOperation;
	//		oOperation.Initialize( psWarpOptions );
	//		oOperation.ChunkAndWarpImage( 0, 0, 
	//			GDALGetRasterXSize( hDstDS ), 
	//			GDALGetRasterYSize( hDstDS ) );
	//		GDALDestroyGenImgProjTransformer( psWarpOptions->pTransformerArg );
	//		GDALDestroyWarpOptions( psWarpOptions );
	//		GDALClose( hDstDS );
	//		GDALClose( hSrcDS );
	//	}

	//	m_srsbase.Close();
	//}

	void make_raster_path(char* rpath, g_uint rsize, const char* repository,  const char* raster_path, const char* raster_name)
	{
		char temp[AUGE_PATH_MAX];
		memset(temp, 0 ,AUGE_PATH_MAX);
		auge_make_path(temp, NULL, repository, raster_path, NULL);
		auge_make_path(rpath, NULL, temp, raster_name, NULL);
	}
}
