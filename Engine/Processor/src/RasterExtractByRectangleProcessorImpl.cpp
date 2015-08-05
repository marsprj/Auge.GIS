#include "RasterExtractByRectangleProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeUser.h"

namespace auge
{
	RasterExtractByRectangleProcessorImpl::RasterExtractByRectangleProcessorImpl()
	{
		m_user = 0;
		m_in_raster_path = "/";
		m_out_raster_path = "/";
	}

	RasterExtractByRectangleProcessorImpl::~RasterExtractByRectangleProcessorImpl()
	{

	}

	void RasterExtractByRectangleProcessorImpl::SetInputRectangle(GEnvelope& rect)
	{
		m_rect = rect;
	}

	void RasterExtractByRectangleProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void RasterExtractByRectangleProcessorImpl::SetInputRaster(const char* rasterName)
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

	void RasterExtractByRectangleProcessorImpl::SetInputPath(const char* rasterPath)
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

	void RasterExtractByRectangleProcessorImpl::SetOutputDataSource(const char* sourceName)
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
	
	void RasterExtractByRectangleProcessorImpl::SetOutputRaster(const char* rasterName)
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

	void RasterExtractByRectangleProcessorImpl::SetOutputPath(const char* rasterPath)
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

	const char*	RasterExtractByRectangleProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	RasterExtractByRectangleProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char* RasterExtractByRectangleProcessorImpl::GetInputRasterPath()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char*	RasterExtractByRectangleProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterExtractByRectangleProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	const char* RasterExtractByRectangleProcessorImpl::GetOutputRasterPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	RESULTCODE RasterExtractByRectangleProcessorImpl::Execute()
	{
		GEnvelope&  inRect = m_rect;
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

		Raster* poutRaster = NULL;
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
		
		GEnvelope& raster_extent = pinRaster->GetExtent();
		GEnvelope exent = raster_extent.Intersect(inRect);

		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();

		poutRaster = pRasterFactory->CreateRaster(outRasterName, exent, pinRaster);
		if(poutRaster==NULL)
		{
			pinRaster->Release();
			pinFolder->Release();
			return AG_FAILURE;
		}

		poutFolder = poutRasterWorkspace->GetFolder(outRasterPath);
		if(poutFolder==NULL)
		{
			pinRaster->Release();
			poutRaster->Release();
			pinFolder->Release();
			return AG_FAILURE;
		}
		RESULTCODE rc = poutFolder->GetRasterDataset()->AddRaster(outRasterName, poutRaster);

		pinRaster->Release();
		poutRaster->Release();
		poutFolder->Release();
		pinFolder->Release();

		return rc;
	}

	void RasterExtractByRectangleProcessorImpl::Release()
	{
		delete this;
	}

	void RasterExtractByRectangleProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}
}
