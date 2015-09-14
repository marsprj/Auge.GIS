#include "RasterSepiaToneEffectProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeUser.h"
#include "math.h"

namespace auge
{
	RasterSepiaToneEffectProcessorImpl::RasterSepiaToneEffectProcessorImpl()
	{
		m_user = 0;
		m_in_raster_path = "/";
		m_out_raster_path = "/";
	}

	RasterSepiaToneEffectProcessorImpl::~RasterSepiaToneEffectProcessorImpl()
	{

	}
	
	void RasterSepiaToneEffectProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void RasterSepiaToneEffectProcessorImpl::SetInputRaster(const char* rasterName)
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

	void RasterSepiaToneEffectProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void RasterSepiaToneEffectProcessorImpl::SetOutputRaster(const char* rasterName)
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
	
	const char*	RasterSepiaToneEffectProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	RasterSepiaToneEffectProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	RasterSepiaToneEffectProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterSepiaToneEffectProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}
	void RasterSepiaToneEffectProcessorImpl::SetInputPath(const char* rasterPath)
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

	void RasterSepiaToneEffectProcessorImpl::SetOutputPath(const char* rasterPath)
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

	const char* RasterSepiaToneEffectProcessorImpl::GetInputRasterPath()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char* RasterSepiaToneEffectProcessorImpl::GetOutputRasterPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	RESULTCODE RasterSepiaToneEffectProcessorImpl::Execute()
	{
		const char* inSourceName = GetInputDataSource();
		const char* inRasterName = GetInputRaster();
		const char* inRasterPath = GetInputRasterPath();
		const char* outSourceName = GetOutputDataSource();
		const char* outRasterName = GetOutputRaster();
		const char* outRasterPath = GetOutputRasterPath();

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

		Raster* pinRaster = NULL;
		RasterFolder* pinFolder = NULL;

		//Raster* poutRaster = NULL;
		RasterFolder* poutFolder = NULL;

		pinFolder = pinRasterWorkspace->GetFolder(inRasterPath);
		if(pinFolder==NULL)
		{
			return AG_FAILURE;
		}
		pinRaster = pinFolder->GetRasterDataset()->GetRaster(inRasterName);
		if(pinRaster==NULL)
		{
			pinFolder->Release();
			return AG_FAILURE;
		}

		g_uint band_count = pinRaster->GetBandCount();
		if(band_count<3||band_count>4)
		{
			pinFolder->Release();
			return AG_FAILURE;
		}


		if(!SepiaTone(pinRaster))
		{
			pinFolder->Release();
			return AG_FAILURE;
		}
		
		poutFolder = poutRasterWorkspace->GetFolder(outRasterPath);
		if(poutFolder==NULL)
		{
			pinFolder->Release();
			return AG_FAILURE;
		}
		RESULTCODE rc = poutFolder->GetRasterDataset()->AddRaster(outRasterName, pinRaster);

		pinRaster->Release();
		poutFolder->Release();
		pinFolder->Release();

		return rc;
	}

	void RasterSepiaToneEffectProcessorImpl::Release()
	{
		delete this;
	}

	bool RasterSepiaToneEffectProcessorImpl::SepiaTone(Raster* pRaster)
	{
		g_uint64 w = pRaster->GetWidth();
		g_uint64 h = pRaster->GetHeight();
		g_uint64 size = w * h;
	
		RasterBand* prBand = pRaster->GetBand(0);
		RasterBand* pgBand = pRaster->GetBand(1);
		RasterBand* pbBand = pRaster->GetBand(2);

		g_uchar* r = (g_uchar*)(prBand->GetData());
		g_uchar* g = (g_uchar*)(pgBand->GetData());
		g_uchar* b = (g_uchar*)(pbBand->GetData());

		g_uchar br, bg, bb;
		double nr, ng, nb;
		double noise; 

		for(g_uint64 i=0; i<size; i++)
		{
			br = r[i];
			bg = g[i];
			bb = b[i];

			nr = ((double)br * 0.393) + ((double)bg * 0.769) + ((double)bb * 0.189);  
			ng = ((double)br * 0.349) + ((double)bg * 0.686) + ((double)bb * 0.168);   
			nb = ((double)br * 0.272) + ((double)bg * 0.534) + ((double)bb * 0.131); 

			noise = ((rand() % 100) * 0.01) * 0.5 + 0.5;
			nr = nr * noise + (1.0-noise)*br;
			ng = ng * noise + (1.0-noise)*bg;
			nb = nb * noise + (1.0-noise)*bb;

			r[i] = nr < 255 ? nr : 255;
			g[i] = ng < 255 ? ng : 255;
			b[i] = nb < 255 ? nb : 255;
		}
		prBand->SetData(r);
		pgBand->SetData(g);
		pbBand->SetData(b);

		//free(r);
		//free(g);
		//free(b);

		return true;
	}

	void RasterSepiaToneEffectProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}
}
