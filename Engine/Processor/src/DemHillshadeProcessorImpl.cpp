#include "DemHillshadeProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeUser.h"

#include <math.h>

namespace auge
{
	DemHillshadeProcessorImpl::DemHillshadeProcessorImpl()
	{
		m_user = 0;
		m_in_raster_path = "/";
		m_out_raster_path = "/";
		m_azimuth= 0.0f;
		m_zenith = 45.0f * AUGE_DEGREE_TO_RADIAN;
		m_zfactor = 1.0f;
	}

	DemHillshadeProcessorImpl::~DemHillshadeProcessorImpl()
	{

	}
	
	void DemHillshadeProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void DemHillshadeProcessorImpl::SetInputRaster(const char* rasterName)
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

	void DemHillshadeProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void DemHillshadeProcessorImpl::SetOutputRaster(const char* rasterName)
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

	const char*	DemHillshadeProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	DemHillshadeProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	DemHillshadeProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	DemHillshadeProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	void DemHillshadeProcessorImpl::SetInputPath(const char* rasterPath)
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

	void DemHillshadeProcessorImpl::SetOutputPath(const char* rasterPath)
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

	const char* DemHillshadeProcessorImpl::GetInputRasterPath()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char* DemHillshadeProcessorImpl::GetOutputRasterPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	void DemHillshadeProcessorImpl::SetAzimuth(float azimuth)
	{
		m_azimuth = azimuth * AUGE_DEGREE_TO_RADIAN;
	}

	void DemHillshadeProcessorImpl::SetZenith(float zenith)
	{
		m_zenith = zenith * AUGE_DEGREE_TO_RADIAN;
	}

	void DemHillshadeProcessorImpl::SetZFactor(float zscale)
	{
		m_zfactor = zscale;
	}

	RESULTCODE DemHillshadeProcessorImpl::Execute()
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

		poutRaster = Hillshade(pinRaster);

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

	void DemHillshadeProcessorImpl::Release()
	{
		delete this;
	}

	void DemHillshadeProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}

	Raster* DemHillshadeProcessorImpl::Hillshade(Raster* pinRaster)
	{
		Raster* poutRaster = NULL;
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();
		poutRaster = pRasterFactory->CreateRaster("", augePixelByte, 1, pinRaster->GetExtent(), pinRaster->GetWidth(), pinRaster->GetHeight(), pinRaster->GetSpatialReference());		
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
				rc = Hillshade_Byte(pinBand, poutBand);
				break;
			case augePixelUInt16:
			case augePixelInt16:
				rc = Hillshade_Short(pinBand, poutBand);
				break;
			case augePixelUInt32:
			case augePixelInt32:
				break;
			case augePixelFloat32:
				break;
			case augePixelDouble:
				rc = Hillshade_Double(pinBand, poutBand);
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

	RESULTCODE DemHillshadeProcessorImpl::Hillshade_Byte(RasterBand* pinBand, RasterBand* poutBand)
	{
		g_uint width = pinBand->GetWidth();
		g_uint height= pinBand->GetHeight();

		g_byte  v_h=0, v_v=0;
		g_byte* input = (g_byte*)pinBand->GetData();
		g_byte* ptr_0 = input + 1; 
		g_byte* ptr_1 = ptr_0 + width;
		g_byte* ptr_2 = ptr_1 + width;
		g_byte  v_min,v_max;
		float slope_x, slope_y,slope;
		float aspect;
		float shade;
		float azimuth_sin;
		float azimuth_cos;
		float azimuth_cos_x_zscale;
		float zenith_cos;
		float zenith_sin;
		float value;
		float reslution_x = pinBand->GetResolution_X();
		float reslution_y = pinBand->GetResolution_Y();

		g_int64 size = width*height*sizeof(float);
		float* output = (float*)malloc(size*sizeof(float));
		memset(output, 0, size);

		zenith_cos = cos(m_zenith);
		zenith_sin = sin(m_zenith);

		g_int sum = 0;
		float* ptr = output + width + 1;
		for(g_uint i=1; i<height-1; i++)
		{	
			for(g_uint j=1; j<width-1; j++,ptr_0++,ptr_1++,ptr_2++,ptr++)
			{
				slope_x = ((ptr_2[ 1] + 2*ptr_1[ 1] + ptr_0[ 1]) - (ptr_2[-1] + 2*ptr_1[-1] + ptr_0[-1])) / (8 * reslution_x);
				slope_y = ((ptr_2[ 1] + 2*ptr_2[ 0] + ptr_2[-1]) - (ptr_0[ 1] + 2*ptr_0[ 0] + ptr_0[-1])) / (8 * reslution_y);

				// slope
				slope = atan(m_zfactor * sqrt(pow(slope_x,2.0f) + pow(slope_y,2.0f)));

				// aspect
				aspect = atan2(slope_y, -slope_x);
				if(aspect<0)
				{
					aspect += TWO_PI;
				}

				//arcgis 10
				//Hillshade = 255.0 * ((cos(Zenith) * cos(Slope)) + (sin(Zenith) * sin(Slope) * cos(Azimuth - Aspect)))			
				shade = 255.0f * ((zenith_cos * cos(slope)) + (zenith_sin * sin(slope) * cos(m_azimuth-aspect)));
				if(shade<0)
				{
					shade = 0.0f;
				}
				*ptr = shade;
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

	RESULTCODE DemHillshadeProcessorImpl::Hillshade_Short(RasterBand* pinBand, RasterBand* poutBand)
	{
		g_uint width = pinBand->GetWidth();
		g_uint height= pinBand->GetHeight();

		short  v_h=0, v_v=0;
		short* input = (short*)pinBand->GetData();
		short* ptr_0 = input + 1; 
		short* ptr_1 = ptr_0 + width;
		short* ptr_2 = ptr_1 + width;
		//short  v_min,v_max;
		float slope_x, slope_y,slope;
		float aspect;
		float shade;
		float zenith_cos;
		float zenith_sin;
		float zscale_2;			//square of zscale
		float reslution_x = pinBand->GetResolution_X();
		float reslution_y = pinBand->GetResolution_Y();

		g_int64 size = width*height*sizeof(g_byte);
		g_byte* output = (g_byte*)malloc(size*sizeof(g_byte));
		memset(output, 0, size);


		zenith_cos = cos(m_zenith);
		zenith_sin = sin(m_zenith);
				
		g_int sum = 0;
		g_byte* ptr = output + width + 1;
		for(g_uint i=1; i<height-1; i++)
		{	
			for(g_uint j=1; j<width-1; j++,ptr_0++,ptr_1++,ptr_2++,ptr++)
			{
				slope_x = ((ptr_2[ 1] + 2*ptr_1[ 1] + ptr_0[ 1]) - (ptr_2[-1] + 2*ptr_1[-1] + ptr_0[-1])) / (8 * reslution_x);
				slope_y = ((ptr_2[ 1] + 2*ptr_2[ 0] + ptr_2[-1]) - (ptr_0[ 1] + 2*ptr_0[ 0] + ptr_0[-1])) / (8 * reslution_y);

				// slope
				slope = atan(m_zfactor * sqrt(pow(slope_x,2.0f) + pow(slope_y,2.0f)));

				// aspect
				aspect = atan2(slope_y, -slope_x);
				if(aspect<0)
				{
					aspect += TWO_PI;
				}

				//arcgis 10
				//Hillshade = 255.0 * ((cos(Zenith) * cos(Slope)) + (sin(Zenith) * sin(Slope) * cos(Azimuth - Aspect)))			
				shade = 255.0f * ((zenith_cos * cos(slope)) + (zenith_sin * sin(slope) * cos(m_azimuth-aspect)));
				if(shade<0)
				{
					shade = 0.0f;
				}
				*ptr = shade;
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


	RESULTCODE DemHillshadeProcessorImpl::Hillshade_Double(RasterBand* pinBand, RasterBand* poutBand)
	{
		g_uint width = pinBand->GetWidth();
		g_uint height= pinBand->GetHeight();

		float  v_h=0, v_v=0;
		float* input = (float*)pinBand->GetData();
		float* ptr_0 = input + 1; 
		float* ptr_1 = ptr_0 + width;
		float* ptr_2 = ptr_1 + width;
		float slope_x, slope_y,slope;
		float aspect;
		float shade;
		float zenith_cos;
		float zenith_sin;
		float value;
		float reslution_x = pinBand->GetResolution_X();
		float reslution_y = pinBand->GetResolution_Y();

		g_int64 size = width*height*sizeof(float);
		float* output = (float*)malloc(size);
		memset(output, 0, size);

		zenith_cos = cos(m_zenith);
		zenith_sin = sin(m_zenith);

		g_int sum = 0;
		float* ptr = output + width + 1;
		for(g_uint i=1; i<height-1; i++)
		{	
			for(g_uint j=1; j<width-1; j++,ptr_0++,ptr_1++,ptr_2++,ptr++)
			{
				slope_x = ((ptr_2[ 1] + 2*ptr_1[ 1] + ptr_0[ 1]) - (ptr_2[-1] + 2*ptr_1[-1] + ptr_0[-1])) / (8 * reslution_x);
				slope_y = ((ptr_2[ 1] + 2*ptr_2[ 0] + ptr_2[-1]) - (ptr_0[ 1] + 2*ptr_0[ 0] + ptr_0[-1])) / (8 * reslution_y);

				// slope
				slope = atan(m_zfactor * sqrt(pow(slope_x,2.0f) + pow(slope_y,2.0f)));

				// aspect
				aspect = atan2(slope_y, -slope_x);
				if(aspect<0)
				{
					aspect += TWO_PI;
				}

				//arcgis 10
				//Hillshade = 255.0 * ((cos(Zenith) * cos(Slope)) + (sin(Zenith) * sin(Slope) * cos(Azimuth - Aspect)))			
				shade = 255.0f * ((zenith_cos * cos(slope)) + (zenith_sin * sin(slope) * cos(m_azimuth-aspect)));
				if(shade<0)
				{
					shade = 0.0f;
				}
				*ptr = shade;
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
