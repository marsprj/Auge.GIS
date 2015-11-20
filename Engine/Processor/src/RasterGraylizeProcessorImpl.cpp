#include "RasterGraylizeProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeUser.h"

namespace auge
{
	RasterGraylizeProcessorImpl::RasterGraylizeProcessorImpl()
	{
		m_user = 0;
		m_in_raster_path = "/";
		m_out_raster_path = "/";
	}

	RasterGraylizeProcessorImpl::~RasterGraylizeProcessorImpl()
	{

	}
	
	void RasterGraylizeProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void RasterGraylizeProcessorImpl::SetInputRaster(const char* rasterName)
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

	void RasterGraylizeProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void RasterGraylizeProcessorImpl::SetOutputRaster(const char* rasterName)
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
	
	const char*	RasterGraylizeProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	RasterGraylizeProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	RasterGraylizeProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterGraylizeProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	void RasterGraylizeProcessorImpl::SetInputPath(const char* rasterPath)
	{
		if(rasterPath==NULL)
		{
			m_in_raster_path = "/";
		}
		else
		{
			if(!strlen(rasterPath))
			{
				m_in_raster_path = "/";
			}
			else
			{
				m_in_raster_path = rasterPath;
			}
		}
	}

	void RasterGraylizeProcessorImpl::SetOutputPath(const char* rasterPath)
	{
		if(rasterPath==NULL)
		{
			m_out_raster_path = "/";
		}
		else
		{
			if(!strlen(rasterPath))
			{
				m_out_raster_path = "/";
			}
			else
			{
				m_out_raster_path = rasterPath;
			}
		}
	}

	const char* RasterGraylizeProcessorImpl::GetInputRasterPath()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char* RasterGraylizeProcessorImpl::GetOutputRasterPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	RESULTCODE RasterGraylizeProcessorImpl::Execute()
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

		GError*  pError  = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();
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
			char msg[AUGE_MSG_MAX];
			memset(msg, 0, AUGE_MSG_MAX);
			g_sprintf(msg, "影像灰度化只支持3波段影像，当前输入影像波段为[%d]个", band_count);
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			pinFolder->Release();
			return AG_FAILURE;
		}


		if(!Greylize(pinRaster))
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

	void RasterGraylizeProcessorImpl::Release()
	{
		delete this;
	}

	bool RasterGraylizeProcessorImpl::Greylize(Raster* pRaster)
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

		//Gray = R*0.299 + G*0.587 + B*0.114
		for(g_uint64 i=0; i<size; i++)
		{
			r[i] = (r[i]*299 + g[i]*587 + b[i]*114) / 1000;
		}
		prBand->SetData(r);
		pgBand->SetData(r);
		pbBand->SetData(r);

		//free(r);
		//free(g);
		//free(b);

		return true;
	}

	void RasterGraylizeProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}
}
