#include "DemSlopeProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeUser.h"

#include <math.h>

namespace auge
{
	DemSlopeProcessorImpl::DemSlopeProcessorImpl()
	{
		m_user = 0;
		m_in_raster_path = "/";
		m_out_raster_path = "/";
	}

	DemSlopeProcessorImpl::~DemSlopeProcessorImpl()
	{

	}
	
	void DemSlopeProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void DemSlopeProcessorImpl::SetInputRaster(const char* rasterName)
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

	void DemSlopeProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void DemSlopeProcessorImpl::SetOutputRaster(const char* rasterName)
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

	const char*	DemSlopeProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	DemSlopeProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	DemSlopeProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	DemSlopeProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	void DemSlopeProcessorImpl::SetInputPath(const char* rasterPath)
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

	void DemSlopeProcessorImpl::SetOutputPath(const char* rasterPath)
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

	const char* DemSlopeProcessorImpl::GetInputRasterPath()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char* DemSlopeProcessorImpl::GetOutputRasterPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	RESULTCODE DemSlopeProcessorImpl::Execute()
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
		if(pinRaster==NULL)
		{
			pinFolder->Release();
			return AG_FAILURE;
		}
		
		poutRaster = Slope(pinRaster);

		if(poutRaster!=NULL)
		{
			poutRaster->SetName(outRasterName);
		}
		poutFolder = poutRasterWorkspace->GetFolder(outRasterPath);
		if(poutFolder==NULL)
		{
			pinFolder->Release();
			poutRaster->Release();
			return AG_FAILURE;
		}
		RESULTCODE rc = poutFolder->GetRasterDataset()->AddRaster(outRasterName, poutRaster);
		poutRaster->Release();
		pinFolder->Release();
		poutFolder->Release();


		return rc;
	}

	void DemSlopeProcessorImpl::Release()
	{
		delete this;
	}

	void DemSlopeProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}

	Raster* DemSlopeProcessorImpl::Slope(Raster* pinRaster)
	{
		Raster* poutRaster = NULL;
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();
		//virtual Raster*			CreateRaster(const char* name, augePixelType pixelType, g_uint bands, GEnvelope& extent, g_uint width, g_uint height, const char*  spatialRef) = 0;
		//poutRaster = pRasterFactory->CreateRaster("", augePixelDouble, pinRaster->GetExtent(), pinRaster);
		//poutRaster = pRasterFactory->CreateRaster("", augePixelDouble, 1, pinRaster->GetExtent(), pinRaster->GetWidth(), pinRaster->GetHeight(), pinRaster->GetSpatialReference());
		poutRaster = pRasterFactory->CreateRaster("", augePixelFloat32, 1, pinRaster->GetExtent(), pinRaster->GetWidth(), pinRaster->GetHeight(), pinRaster->GetSpatialReference());
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
				rc = Slope_Byte(pinBand, poutBand);
				break;
			case augePixelUInt16:
			case augePixelInt16:
				rc = Slope_Short(pinBand, poutBand);
				break;
			case augePixelUInt32:
			case augePixelInt32:
				break;
			case augePixelFloat32:
				break;
			case augePixelDouble:
				rc = Slope_Double(pinBand, poutBand);
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

	RESULTCODE DemSlopeProcessorImpl::Slope_Byte(RasterBand* pinBand, RasterBand* poutBand)
	{
		g_uint width = pinBand->GetWidth();
		g_uint height= pinBand->GetHeight();

		g_byte  v_h=0, v_v=0;
		g_byte* input = (g_byte*)pinBand->GetData();
		g_byte* ptr_0 = input + 1; 
		g_byte* ptr_1 = ptr_0 + width;
		g_byte* ptr_2 = ptr_1 + width;
		g_byte  v_min,v_max;
		float slope_x, slope_y;
		float value;
		float reslution_x = pinBand->GetResolution_X();
		float reslution_y = pinBand->GetResolution_Y();

		g_int64 size = ((g_int64)width) * ((g_int64)height) * poutBand->GetPixelSize();
		float* output = (float*)malloc(size);
		memset(output, 0, size);

		g_int sum = 0;
		float* ptr = output + width + 1;
		for(g_uint i=1; i<height-1; i++)
		{	
			for(g_uint j=1; j<width-1; j++,ptr_0++,ptr_1++,ptr_2++,ptr++)
			{
				slope_x = ((ptr_2[-1] + 2*ptr_1[-1] + ptr_0[-1]) - (ptr_2[ 1] + 2*ptr_1[ 1] + ptr_0[ 1])) / (8 * reslution_x);
				slope_y = ((ptr_2[ 1] + 2*ptr_2[ 0] + ptr_2[-1]) - (ptr_0[ 1] + 2*ptr_0[ 0] + ptr_0[-1])) / (8 * reslution_y);

				value = sqrt(pow(slope_x,2.0f) + pow(slope_y,2.0f));
				*ptr  = atan(value) * AUGE_RADIAN_TO_DEGREE;				
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

	RESULTCODE DemSlopeProcessorImpl::Slope_Short(RasterBand* pinBand, RasterBand* poutBand)
	{
		g_uint width = pinBand->GetWidth();
		g_uint height= pinBand->GetHeight();

		short  v_h=0, v_v=0;
		short* input = (short*)pinBand->GetData();
		short* ptr_0 = input + 1; 
		short* ptr_1 = ptr_0 + width;
		short* ptr_2 = ptr_1 + width;
		short  v_min,v_max;
		float slope_x, slope_y;
		float value;
		float radian;
		float reslution_x = pinBand->GetResolution_X();
		float reslution_y = pinBand->GetResolution_Y();
		
		g_int64 size = ((g_int64)width) * ((g_int64)height) * poutBand->GetPixelSize();
		float* output = (float*)malloc(size);
		memset(output, 0, size);
		
		g_int sum = 0;
		float* ptr = output + width + 1;
		for(g_uint i=1; i<height-1; i++)
		{	
			for(g_uint j=1; j<width-1; j++,ptr_0++,ptr_1++,ptr_2++,ptr++)
			{
				slope_x = ((ptr_2[-1] + 2*ptr_1[-1] + ptr_0[-1]) - (ptr_2[ 1] + 2*ptr_1[ 1] + ptr_0[ 1])) / (8 * reslution_x);
				slope_y = ((ptr_2[ 1] + 2*ptr_2[ 0] + ptr_2[-1]) - (ptr_0[ 1] + 2*ptr_0[ 0] + ptr_0[-1])) / (8 * reslution_y);

				value = sqrt(pow(slope_x,2) + pow(slope_y, 2));
				radian = atan(value);
				*ptr  = radian * AUGE_RADIAN_TO_DEGREE;

			}
			ptr_0 += 2;
			ptr_1 += 2;
			ptr_2 += 2;
			ptr  += 2;		
		}

		float p_0_0, p_0_1, p_0_2;
		float p_1_0, p_1_1, p_1_2;
		
		poutBand->SetData(output);
		free(output);

		return AG_SUCCESS;
	}

	RESULTCODE DemSlopeProcessorImpl::Slope_Float(RasterBand* pinBand, RasterBand* poutBand)
	{
		g_uint width = pinBand->GetWidth();
		g_uint height= pinBand->GetHeight();

		float  v_h=0, v_v=0;
		float* input = (float*)pinBand->GetData();
		float* ptr_0 = input + 1; 
		float* ptr_1 = ptr_0 + width;
		float* ptr_2 = ptr_1 + width;
		float  v_min,v_max;
		float slope_x, slope_y;
		float value;
		float reslution_x = pinBand->GetResolution_X();
		float reslution_y = pinBand->GetResolution_Y();

		g_int64 size = width*height*sizeof(float);
		float* output = (float*)malloc(size);
		memset(output, 0, size);

		g_int sum = 0;
		float* ptr = output + width + 1;
		for(g_uint i=1; i<height-1; i++)
		{	
			for(g_uint j=1; j<width-1; j++,ptr_0++,ptr_1++,ptr_2++,ptr++)
			{
				slope_x = ((ptr_2[-1] + 2*ptr_1[-1] + ptr_0[-1]) - (ptr_2[ 1] + 2*ptr_1[ 1] + ptr_0[ 1])) / (8 * reslution_x);
				slope_y = ((ptr_2[ 1] + 2*ptr_2[ 0] + ptr_2[-1]) - (ptr_0[ 1] + 2*ptr_0[ 0] + ptr_0[-1])) / (8 * reslution_y);

				value = sqrt(pow(slope_x,2) + pow(slope_y, 2));
				*ptr  = atan(value) * AUGE_RADIAN_TO_DEGREE;
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

	RESULTCODE DemSlopeProcessorImpl::Slope_Double(RasterBand* pinBand, RasterBand* poutBand)
	{
		g_uint width = pinBand->GetWidth();
		g_uint height= pinBand->GetHeight();

		float  v_h=0, v_v=0;
		double* input = (double*)pinBand->GetData();
		double* ptr_0 = input + 1; 
		double* ptr_1 = ptr_0 + width;
		double* ptr_2 = ptr_1 + width;
		double  v_min,v_max;
		float slope_x, slope_y;
		float value;
		double reslution_x = pinBand->GetResolution_X();
		double reslution_y = pinBand->GetResolution_Y();

		g_int64 size = width*height*sizeof(float);
		float* output = (float*)malloc(size);
		memset(output, 0, size);

		g_int sum = 0;
		float* ptr = output + width + 1;
		for(g_uint i=1; i<height-1; i++)
		{	
			for(g_uint j=1; j<width-1; j++,ptr_0++,ptr_1++,ptr_2++,ptr++)
			{
				slope_x = ((ptr_2[-1] + 2*ptr_1[-1] + ptr_0[-1]) - (ptr_2[ 1] + 2*ptr_1[ 1] + ptr_0[ 1])) / (8 * reslution_x);
				slope_y = ((ptr_2[ 1] + 2*ptr_2[ 0] + ptr_2[-1]) - (ptr_0[ 1] + 2*ptr_0[ 0] + ptr_0[-1])) / (8 * reslution_y);
				
				value = sqrt(pow(slope_x,2.0f) + pow(slope_y, 2.0f));
				*ptr  = atan(value) * AUGE_RADIAN_TO_DEGREE;
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
