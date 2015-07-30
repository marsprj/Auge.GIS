#include "RasterSubtractProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeUser.h"

#include <math.h>

namespace auge
{
	RasterSubtractProcessorImpl::RasterSubtractProcessorImpl()
	{
		m_user = 0;
	}

	RasterSubtractProcessorImpl::~RasterSubtractProcessorImpl()
	{

	}
	
	void RasterSubtractProcessorImpl::SetInputDataSource_1(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_in_source_name_1.clear();
		}
		else
		{
			m_in_source_name_1 = sourceName;
		}
	}

	void RasterSubtractProcessorImpl::SetInputRaster_1(const char* rasterName)
	{
		if(rasterName==NULL)
		{
			m_in_raster_name_1.clear();
		}
		else
		{
			m_in_raster_name_1 = rasterName;
		}
	}

	void RasterSubtractProcessorImpl::SetInputDataSource_2(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_in_source_name_2.clear();
		}
		else
		{
			m_in_source_name_2 = sourceName;
		}
	}

	void RasterSubtractProcessorImpl::SetInputRaster_2(const char* rasterName)
	{
		if(rasterName==NULL)
		{
			m_in_raster_name_2.clear();
		}
		else
		{
			m_in_raster_name_2 = rasterName;
		}
	}
	void RasterSubtractProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void RasterSubtractProcessorImpl::SetOutputRaster(const char* rasterName)
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

	const char*	RasterSubtractProcessorImpl::GetInputDataSource_1()
	{
		return m_in_source_name_1.empty() ? NULL : m_in_source_name_1.c_str();
	}

	const char*	RasterSubtractProcessorImpl::GetInputRaster_1()
	{
		return m_in_raster_name_1.empty() ? NULL : m_in_raster_name_1.c_str();
	}

	const char*	RasterSubtractProcessorImpl::GetInputDataSource_2()
	{
		return m_in_source_name_2.empty() ? NULL : m_in_source_name_2.c_str();
	}

	const char*	RasterSubtractProcessorImpl::GetInputRaster_2()
	{
		return m_in_raster_name_2.empty() ? NULL : m_in_raster_name_2.c_str();
	}

	const char*	RasterSubtractProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterSubtractProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	RESULTCODE RasterSubtractProcessorImpl::Execute()
	{
		const char* inSourceName_1 = GetInputDataSource_1();
		const char* inRasterName_1 = GetInputRaster_1();
		const char* inSourceName_2 = GetInputDataSource_2();
		const char* inRasterName_2 = GetInputRaster_2();
		const char* outSourceName = GetOutputDataSource();
		const char* outRasterName = GetOutputRaster();

		Workspace* pWorkspace = NULL;
		RasterWorkspace* pinRasterWorkspace_1 = NULL;
		RasterWorkspace* pinRasterWorkspace_2 = NULL;
		RasterWorkspace* poutRasterWorkspace = NULL;

		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = pConnManager->GetWorkspace(m_user, inSourceName_1);
		if(pWorkspace==NULL)
		{
			return AG_FAILURE;
		}
		pinRasterWorkspace_1 = dynamic_cast<RasterWorkspace*>(pWorkspace);

		pWorkspace = pConnManager->GetWorkspace(m_user, inSourceName_2);
		if(pWorkspace==NULL)
		{
			return AG_FAILURE;
		}
		pinRasterWorkspace_2 = dynamic_cast<RasterWorkspace*>(pWorkspace);

		pWorkspace = pConnManager->GetWorkspace(m_user, outSourceName);
		if(pWorkspace==NULL)
		{
			return AG_FAILURE;
		}
		poutRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

		Raster* pinRaster_1 = NULL;
		RasterDataset* pinRasterDataset_1 = NULL;
		Raster* pinRaster_2 = NULL;
		RasterDataset* pinRasterDataset_2 = NULL;

		Raster* poutRaster = NULL;
		RasterDataset* poutRasterDataset = NULL;

		// Get Raster 1
		pinRasterDataset_1 = pinRasterWorkspace_1->OpenRasterDataset(inRasterName_1);
		if(pinRasterDataset_1==NULL)
		{
			return AG_FAILURE;
		}
		pinRaster_1 = pinRasterDataset_1->GetRaster();
		if(pinRaster_1==NULL)
		{
			pinRasterDataset_1->Release();
			return AG_FAILURE;
		}

		// Get Raster 2
		pinRasterDataset_2 = pinRasterWorkspace_2->OpenRasterDataset(inRasterName_2);
		if(pinRasterDataset_1==NULL)
		{
			return AG_FAILURE;
		}
		pinRaster_2 = pinRasterDataset_2->GetRaster();
		if(pinRaster_2==NULL)
		{
			pinRasterDataset_1->Release();
			pinRasterDataset_2->Release();
			return AG_FAILURE;
		}

		g_uint bands_1 = pinRaster_1->GetBandCount();
		g_uint bands_2 = pinRaster_2->GetBandCount();
		if(bands_1!=bands_2)
		{
			pinRasterDataset_1->Release();
			pinRasterDataset_2->Release();
			return AG_FAILURE;
		}

		g_uint width_1 = pinRaster_1->GetWidth();
		g_uint width_2 = pinRaster_2->GetWidth();
		if(width_1 != width_2)
		{
			pinRasterDataset_1->Release();
			pinRasterDataset_2->Release();
			return AG_FAILURE;
		}

		g_uint height_1 = pinRaster_1->GetHeight();
		g_uint height_2 = pinRaster_2->GetHeight();
		if(height_1 != height_1)
		{
			pinRasterDataset_1->Release();
			pinRasterDataset_2->Release();
			return AG_FAILURE;
		}

		poutRaster = Subtract(pinRaster_1, pinRaster_2);

		if(poutRaster!=NULL)
		{
			poutRaster->SetName(outRasterName);
		}
		RESULTCODE rc = poutRasterWorkspace->AddRaster(poutRaster);
		poutRaster->Release();
		pinRasterDataset_1->Release();
		pinRasterDataset_2->Release();

		return rc;
	}

	void RasterSubtractProcessorImpl::Release()
	{
		delete this;
	}

	void RasterSubtractProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}

	Raster* RasterSubtractProcessorImpl::Subtract(Raster* pinRaster_1, Raster* pinRaster_2)
	{
		RasterBand* pinBand_1  = NULL;
		RasterBand* pinBand_2  = NULL;
		Raster* poutRaster = NULL;

		int band_count = pinRaster_1->GetBandCount();
		for(int i=0; i<band_count; i++)
		{
			pinBand_1 = pinRaster_1->GetBand(i);
			pinBand_2 = pinRaster_2->GetBand(i);
			Subtract(pinBand_1, pinBand_2);			
		}

		poutRaster = pinRaster_1;
		poutRaster->AddRef();
		return poutRaster;
	}

	RESULTCODE RasterSubtractProcessorImpl::Subtract(RasterBand* pinBand_1, RasterBand* pinBand_2)
	{
		g_uint width = pinBand_1->GetWidth();
		g_uint height= pinBand_2->GetHeight();

		g_byte v_h=0, v_v=0;
		g_byte* data_1 = (g_byte*)pinBand_1->GetData();
		g_byte* data_2 = (g_byte*)pinBand_2->GetData();
		g_byte* ptr_1 = data_1;
		g_byte* ptr_2 = data_2;
		
		for(g_uint i=0; i<height; i++)
		{	
			for(g_uint j=0; j<width; j++, ptr_1++, ptr_2++)
			{
				*ptr_1 = *ptr_1 - *ptr_2;
			}
		}
		pinBand_1->SetData(data_1);
		return AG_SUCCESS;
	}
}