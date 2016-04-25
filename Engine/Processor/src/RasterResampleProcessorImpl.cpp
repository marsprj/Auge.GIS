#include "RasterResampleProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeUser.h"

#include "gdal/gdalwarper.h"
#include "gdal/cpl_conv.h"
#include "gdal/gdal_priv.h"
#include "gdal/ogr_spatialref.h"

#include <math.h>

#include <fstream>

namespace auge
{
	void make_raster_path(char* rpath, g_uint rsize, const char* repository,  const char* raster_path, const char* raster_name);
	const char* auge_raster_get_driver(const char* ext)
	{
		if(strlen(ext)==0)
		{
			return "";
		}
		const char* suffix = ext[0]=='.' ? ext+1 : ext;
		if(g_stricmp(suffix,"png")==0)
		{
			return "png";
		}
		else if((g_stricmp(suffix,"jpg")==0)||(g_stricmp(suffix,"jpeg")==0))
		{
			return "jpeg";
		}
		else if((g_stricmp(suffix,"tif")==0)||(g_stricmp(suffix,"tiff")==0))
		{
			return "GTiff";
		}
		return "";
	}

	RasterResampleProcessorImpl::RasterResampleProcessorImpl()
	{
		m_user = 0;
		m_in_raster_path = "/";
		m_out_raster_path = "/";
	}

	RasterResampleProcessorImpl::~RasterResampleProcessorImpl()
	{

	}
	
	void RasterResampleProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void RasterResampleProcessorImpl::SetInputRaster(const char* rasterName)
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

	void RasterResampleProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void RasterResampleProcessorImpl::SetOutputRaster(const char* rasterName)
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

	const char*	RasterResampleProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	RasterResampleProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	RasterResampleProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterResampleProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	void RasterResampleProcessorImpl::SetInputPath(const char* rasterPath)
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

	void RasterResampleProcessorImpl::SetOutputPath(const char* rasterPath)
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

	const char* RasterResampleProcessorImpl::GetInputRasterPath()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char* RasterResampleProcessorImpl::GetOutputRasterPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	RESULTCODE RasterResampleProcessorImpl::Execute()
	{
		RESULTCODE rc = AG_SUCCESS;

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

		


		return rc;
	}

	void RasterResampleProcessorImpl::Release()
	{
		delete this;
	}

	void RasterResampleProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}

	RESULTCODE RasterResampleProcessorImpl::Resample(const char* inpath, const char* outpath, float scale/*=1.0*/)
	{
		GDALAllRegister(); 

		//Ë«ÏßÐÔ²åÖµ
		GDALResampleAlg eResample = GRA_Bilinear;

		GDALDataset *psDataset = NULL;
		GDALDataset *poDataset = NULL;
		GDALDriver  *poDriver  = NULL;

		char ext[AUGE_EXT_MAX];
		memset(ext,0,AUGE_EXT_MAX);
		auge_split_path(outpath, NULL, NULL,NULL,ext);
		const char* format = auge_raster_get_driver(ext);
		poDriver = GetGDALDriverManager()->GetDriverByName(format);
		if(poDriver==NULL)
		{
			return AG_FAILURE;
		}
		
		psDataset = (GDALDataset *)GDALOpen(inpath, GA_ReadOnly);
		if(psDataset==NULL)
		{
			return AG_FAILURE;
		}
		
		int nbands = psDataset->GetRasterCount();
		const char* sswkt = psDataset->GetProjectionRef();
		GDALDataType dataType = psDataset->GetRasterBand(1)->GetRasterDataType();
		double dGeoTrans[6] = {0};
		psDataset->GetGeoTransform(dGeoTrans);

		dGeoTrans[1] = dGeoTrans[1] / scale;
		dGeoTrans[5] = dGeoTrans[5] / scale;

		int nwidth = (psDataset->GetRasterXSize() * scale + 0.5);
		int nheight= (psDataset->GetRasterYSize() * scale + 0.5);

		poDataset = poDriver->Create(outpath,nwidth,nheight,nbands,dataType,NULL);
		if(poDataset==NULL)
		{
			GDALClose(psDataset);
			return AG_FAILURE;
		}

		poDataset->SetProjection(sswkt);
		poDataset->SetGeoTransform(dGeoTrans);

		//////////////////////////////////////////////////////////////////////////
		int *pSrcBand = new int[nbands];  
		int *pDstBand = new int[nbands]; 
		for (int i=0; i<nbands; i++)  
		{  
			pSrcBand[i] = i+1;  
			pDstBand[i] = i+1;  
		}  

		void *hTransformArg = NULL, *hGenImgPrjArg = NULL;  
		hTransformArg = hGenImgPrjArg = GDALCreateGenImgProjTransformer2((GDALDatasetH) psDataset, (GDALDatasetH) poDataset, NULL);  
		if (hTransformArg == NULL)  
		{  

			GDALClose((GDALDatasetH) psDataset);  
			GDALClose((GDALDatasetH) poDataset);  
			return AG_FAILURE;  
		}  

		GDALTransformerFunc pFnTransformer = GDALGenImgProjTransform;  
		GDALWarpOptions *psWo = GDALCreateWarpOptions();  

		psWo->papszWarpOptions = CSLDuplicate(NULL);  
		psWo->eWorkingDataType = dataType;  
		psWo->eResampleAlg = eResample;  

		psWo->hSrcDS = (GDALDatasetH) psDataset;  
		psWo->hDstDS = (GDALDatasetH) poDataset;  

		psWo->pfnTransformer = pFnTransformer;  
		psWo->pTransformerArg = hTransformArg;  

		psWo->pfnProgress = GDALTermProgress;  
		psWo->pProgressArg = GDALCreateGenImgProjTransformer( (GDALDatasetH)psDataset, 
			GDALGetProjectionRef((GDALDatasetH)psDataset), 
			(GDALDatasetH)poDataset,
			GDALGetProjectionRef((GDALDatasetH)poDataset), 
			FALSE, 0.0, 1 );

		psWo->nBandCount = nbands;  
		psWo->panSrcBands = (int *) CPLMalloc(nbands*sizeof(int));  
		psWo->panDstBands = (int *) CPLMalloc(nbands*sizeof(int));  
		for (int i=0; i<nbands; i++)  
		{  
			psWo->panSrcBands[i] = pSrcBand[i];  
			psWo->panDstBands[i] = pDstBand[i];  
		}  
		delete[] pSrcBand;  
		delete[] pDstBand;  

		GDALWarpOperation oWo;  
		if (oWo.Initialize(psWo) != CE_None)  
		{  
			GDALClose((GDALDatasetH) psDataset);  
			GDALClose((GDALDatasetH) poDataset);  

			return AG_FAILURE;  
		}  

		oWo.ChunkAndWarpImage(0, 0, nwidth, nheight);  

		GDALDestroyGenImgProjTransformer(psWo->pTransformerArg);  
		GDALDestroyWarpOptions( psWo );  
		GDALClose((GDALDatasetH) psDataset);  
		GDALClose((GDALDatasetH) poDataset);  

		return AG_SUCCESS;
	}

}
