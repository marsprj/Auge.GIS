#include "RasterSmoothProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeUser.h"

#include <math.h>
#include <fstream>

namespace auge
{
	int mean_filter[3][3] ={1, 1, 1,
							1, 1, 1,
							1, 1, 1};

	int gauss_filter[3][3]={1, 2, 1,
							2, 4, 2,
							1, 2, 1};

	RasterSmoothProcessorImpl::RasterSmoothProcessorImpl()
	{
		m_user = 0;
	}

	RasterSmoothProcessorImpl::~RasterSmoothProcessorImpl()
	{

	}
	
	void RasterSmoothProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void RasterSmoothProcessorImpl::SetInputRaster(const char* rasterName)
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

	void RasterSmoothProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void RasterSmoothProcessorImpl::SetOutputRaster(const char* rasterName)
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
	
	void RasterSmoothProcessorImpl::SetSmoother(augeRasterSmoother smoother)
	{
		m_smoother = smoother;
	}

	const char*	RasterSmoothProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	RasterSmoothProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	RasterSmoothProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterSmoothProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	void RasterSmoothProcessorImpl::SetInputPath(const char* rasterPath)
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

	void RasterSmoothProcessorImpl::SetOutputPath(const char* rasterPath)
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

	const char* RasterSmoothProcessorImpl::GetInputRasterPath()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char* RasterSmoothProcessorImpl::GetOutputRasterPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	RESULTCODE RasterSmoothProcessorImpl::Execute()
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

		Raster* poutRaster = NULL;
		RasterFolder* poutFolder = NULL;

		pinFolder = pinRasterWorkspace->GetFolder(inRasterPath);
		if(pinFolder==NULL)
		{
			return AG_FAILURE;
		}
		pinRaster = pinFolder->GetRasterDataset()->GetRaster(inRasterName);
		
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();

		switch(m_smoother)
		{
		case augeSmoothGauss:
			poutRaster = Smooth(pinRaster, gauss_filter, 16);
			break;
		case augeSmoothMean:
			poutRaster = Smooth(pinRaster, mean_filter, 9);
			break;
		case augeSmoothMedian:
			poutRaster = Median(pinRaster);
			break;
		case augeSmoothBilateral:
			break;
		}
		
		if(poutRaster!=NULL)
		{
			poutRaster->SetName(outRasterName);
		}
		poutFolder = poutRasterWorkspace->GetFolder(outRasterPath);
		if(poutFolder==NULL)
		{
			poutRaster->Release();
			pinFolder->Release();
			return AG_FAILURE;
		}
		RESULTCODE rc = poutFolder->GetRasterDataset()->AddRaster(outRasterName, poutRaster);

		poutRaster->Release();
		poutFolder->Release();
		pinFolder->Release();

		return rc;
	}

	void RasterSmoothProcessorImpl::Release()
	{
		delete this;
	}


	void RasterSmoothProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}
	
	Raster* RasterSmoothProcessorImpl::Smooth(Raster* pinRaster, int filter[][3], int denominator)
	{
		Raster* poutRaster = NULL;
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();
		poutRaster = pRasterFactory->CreateRaster("", pinRaster->GetExtent(), pinRaster);
		if(poutRaster==NULL)
		{
			return NULL;
		}

		RESULTCODE rc = AG_SUCCESS;
		augePixelType type = pinRaster->GetPixelType();

		RasterBand* pinBand = NULL;
		RasterBand* poutBand = NULL;
		g_uint band_count = pinRaster->GetBandCount();
		for(g_uint i=0; i<band_count; i++)
		{
			pinBand  = pinRaster->GetBand(i);
			poutBand = poutRaster->GetBand(i);

			switch(type)
			{
			case augePixelByte:
				rc = Smooth_Byte(pinBand, poutBand, filter, denominator);
				break;
			case augePixelUInt16:
			case augePixelInt16:
				break;
			case augePixelUInt32:
			case augePixelInt32:
				break;
			case augePixelFloat32:
				break;
			case augePixelDouble:
				break;
			}

			if(rc!=AG_SUCCESS)
			{
				break;
			}
		}
		
		if(rc!=AG_SUCCESS)
		{
			poutRaster->Release();
			poutRaster = NULL;
		}

		return poutRaster;
	}

	RESULTCODE RasterSmoothProcessorImpl::Smooth_Byte(RasterBand* pinBand, RasterBand* poutBand, int filter[][3], int denominator)
	{
		g_uint width = pinBand->GetWidth();
		g_uint height= pinBand->GetHeight();

		g_byte v_h=0, v_v=0;
		g_byte* input = (g_byte*)pinBand->GetData();
		g_byte* ptr_0 = input + 1; 
		g_byte* ptr_1 = ptr_0 + width;
		g_byte* ptr_2 = ptr_1 + width;
		g_byte  v_min,v_max;
		
		g_int64 size = width*height*sizeof(g_byte);
		g_byte* output = (g_byte*)malloc(size);
		memcpy(output, input, size);

		g_int sum = 0;
		g_byte* ptr = output + width + 1;
		for(g_uint i=1; i<height-1; i++)
		{	
			for(g_uint j=1; j<width-1; j++,ptr_0++,ptr_1++,ptr_2++,ptr++)
			{
				sum = filter[0][0]*ptr_0[-1] + filter[0][1]*ptr_0[0] + filter[0][2]*ptr_0[1]
					+ filter[1][0]*ptr_1[-1] + filter[1][1]*ptr_1[0] + filter[1][2]*ptr_1[1]
					+ filter[2][0]*ptr_2[-1] + filter[2][1]*ptr_2[0] + filter[2][2]*ptr_2[1];

				*ptr = sum / denominator;
			}
			ptr_0 += 2;
			ptr_1 += 2;
			ptr_2 += 2;
			ptr  += 2;
		}

		poutBand->SetData(output);
		free(output);

		return AG_SUCCESS;
	}

	Raster*	RasterSmoothProcessorImpl::Median(Raster* pinRaster)
	{
		Raster* poutRaster = NULL;
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();
		poutRaster = pRasterFactory->CreateRaster("", pinRaster->GetExtent(), pinRaster);
		if(poutRaster==NULL)
		{
			return NULL;
		}

		RESULTCODE rc = AG_SUCCESS;
		augePixelType type = pinRaster->GetPixelType();

		RasterBand* pinBand = NULL;
		RasterBand* poutBand = NULL;
		g_uint band_count = pinRaster->GetBandCount();
		for(g_uint i=0; i<band_count; i++)
		{
			pinBand  = pinRaster->GetBand(i);
			poutBand = poutRaster->GetBand(i);

			switch(type)
			{
			case augePixelByte:
				rc = Median_Byte(pinBand, poutBand);
				break;
			case augePixelUInt16:
			case augePixelInt16:
				break;
			case augePixelUInt32:
			case augePixelInt32:
				break;
			case augePixelFloat32:
				break;
			case augePixelDouble:
				break;
			}

			if(rc!=AG_SUCCESS)
			{
				break;
			}
		}

		if(rc!=AG_SUCCESS)
		{
			poutRaster->Release();
			poutRaster = NULL;
		}

		return poutRaster;
	}

	RESULTCODE	RasterSmoothProcessorImpl::Median_Byte(RasterBand* pinBand, RasterBand* poutBand)
	{
		g_uint width = pinBand->GetWidth();
		g_uint height= pinBand->GetHeight();

		g_byte v_h=0, v_v=0;
		g_byte* input = (g_byte*)pinBand->GetData();
		g_byte* ptr_0 = input + 1; 
		g_byte* ptr_1 = ptr_0 + width;
		g_byte* ptr_2 = ptr_1 + width;
		g_byte  v_min,v_max;

		g_int64 size = width*height*sizeof(g_byte);
		g_byte* output = (g_byte*)malloc(size);
		memcpy(output, input, size);

		g_int sum = 0;
		g_byte* ptr = output + width + 1;
		for(g_uint i=1; i<height-1; i++)
		{	
			for(g_uint j=1; j<width-1; j++,ptr_0++,ptr_1++,ptr_2++,ptr++)
			{
				g_byte values[9] = {ptr_0[-1] , ptr_0[0] , ptr_0[1],
									ptr_1[-1] , ptr_1[0] , ptr_1[1],
									ptr_2[-1] , ptr_2[0] , ptr_2[1]};
				*ptr = auge_get_median(values, 9);

			}
			ptr_0 += 2;
			ptr_1 += 2;
			ptr_2 += 2;
			ptr  += 2;
		}

		poutBand->SetData(output);
		free(output);

		return AG_SUCCESS;
	}

}
