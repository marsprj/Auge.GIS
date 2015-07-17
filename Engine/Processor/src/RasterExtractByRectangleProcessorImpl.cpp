#include "RasterExtractByRectangleProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeUser.h"

namespace auge
{
	RasterExtractByRectangleProcessorImpl::RasterExtractByRectangleProcessorImpl()
	{
		m_pUser = NULL;
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

	const char*	RasterExtractByRectangleProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	RasterExtractByRectangleProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	RasterExtractByRectangleProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterExtractByRectangleProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	RESULTCODE RasterExtractByRectangleProcessorImpl::Execute()
	{
		GEnvelope&  inRect = m_rect;
		const char* inSourceName = GetInputDataSource();
		const char* inRasterName = GetInputRaster();
		const char* outSourceName = GetOutputDataSource();
		const char* outRasterName = GetOutputRaster();

		Workspace* pWorkspace = NULL;
		RasterWorkspace* pinRasterWorkspace = NULL;
		RasterWorkspace* poutRasterWorkspace = NULL;

		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = pConnManager->GetWorkspace(m_pUser->GetID(), inSourceName);
		if(pWorkspace==NULL)
		{
			return AG_FAILURE;
		}
		pinRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

		pWorkspace = pConnManager->GetWorkspace(m_pUser->GetID(), outSourceName);
		if(pWorkspace==NULL)
		{
			return AG_FAILURE;
		}
		poutRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);


		Raster* pinRaster = NULL;
		RasterDataset* pinRasterDataset = NULL;

		Raster* poutRaster = NULL;
		RasterDataset* poutRasterDataset = NULL;

		pinRasterDataset = pinRasterWorkspace->OpenRasterDataset(inRasterName);
		if(pinRasterDataset==NULL)
		{
			return AG_FAILURE;
		}
		pinRaster = pinRasterDataset->GetRaster();
		
		GEnvelope& raster_extent = pinRaster->GetExtent();
		GEnvelope exent = raster_extent.Intersect(inRect);

		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();

		poutRaster = pRasterFactory->CreateRaster(outRasterName, exent, pinRaster);
		if(poutRaster==NULL)
		{
			pinRasterDataset->Release();
			return AG_FAILURE;
		}

		RESULTCODE rc = poutRasterWorkspace->AddRaster(poutRaster);
		poutRaster->Release();
		pinRasterDataset->Release();

		return rc;
	}

	void RasterExtractByRectangleProcessorImpl::Release()
	{
		delete this;
	}

	void RasterExtractByRectangleProcessorImpl::SetUser(User* pUser)
	{
		m_pUser = pUser;
	}
}
