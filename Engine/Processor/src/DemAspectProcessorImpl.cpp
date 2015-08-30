#include "DemAspectProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeUser.h"

#include <math.h>

namespace auge
{
	DemAspectProcessorImpl::DemAspectProcessorImpl()
	{
		m_user = 0;
		m_in_raster_path = "/";
		m_out_raster_path = "/";
	}

	DemAspectProcessorImpl::~DemAspectProcessorImpl()
	{

	}
	
	void DemAspectProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void DemAspectProcessorImpl::SetInputRaster(const char* rasterName)
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

	void DemAspectProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void DemAspectProcessorImpl::SetOutputRaster(const char* rasterName)
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

	const char*	DemAspectProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	DemAspectProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	DemAspectProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	DemAspectProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	void DemAspectProcessorImpl::SetInputPath(const char* rasterPath)
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

	void DemAspectProcessorImpl::SetOutputPath(const char* rasterPath)
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

	const char* DemAspectProcessorImpl::GetInputRasterPath()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char* DemAspectProcessorImpl::GetOutputRasterPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	RESULTCODE DemAspectProcessorImpl::Execute()
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

		
		poutRaster = Aspect(pinRaster);

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

	void DemAspectProcessorImpl::Release()
	{
		delete this;
	}

	void DemAspectProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}

	Raster* DemAspectProcessorImpl::Aspect(Raster* pinRaster)
	{
		Raster* poutRaster = NULL;
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();
		poutRaster = pRasterFactory->CreateRaster("", augePixelDouble, 1, pinRaster->GetExtent(), pinRaster->GetWidth(), pinRaster->GetHeight(), pinRaster->GetSpatialReference());
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
				rc = Aspect_Byte(pinBand, poutBand);
				break;
			case augePixelUInt16:
			case augePixelInt16:
				Aspect_Short(pinBand, poutBand);
				break;
			case augePixelUInt32:
			case augePixelInt32:
				break;
			case augePixelFloat32:
				break;
			case augePixelDouble:
				Aspect_Double(pinBand, poutBand);
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

	RESULTCODE DemAspectProcessorImpl::Aspect_Byte(RasterBand* pinBand, RasterBand* poutBand)
	{
		g_uint width = pinBand->GetWidth();
		g_uint height= pinBand->GetHeight();

		g_byte  v_h=0, v_v=0;
		g_byte* input = (g_byte*)pinBand->GetData();
		g_byte* ptr_0 = input + 1; 
		g_byte* ptr_1 = ptr_0 + width;
		g_byte* ptr_2 = ptr_1 + width;
		g_byte  v_min,v_max;
		double slope_x, slope_y;
		double aspect;
		double value;
		double reslution_x = pinBand->GetResolution_X();
		double reslution_y = pinBand->GetResolution_Y();

		g_int64 size = width*height*sizeof(g_byte);
		double* output = (double*)malloc(size*sizeof(double));
		memset(output, 0, size*sizeof(double));

		g_int sum = 0;
		double* ptr = output + width + 1;
		for(g_uint i=1; i<height-1; i++)
		{	
			for(g_uint j=1; j<width-1; j++,ptr_0++,ptr_1++,ptr_2++,ptr++)
			{
				slope_x = ((ptr_2[-1] + 2*ptr_1[-1] + ptr_0[-1]) - (ptr_2[ 1] + 2*ptr_1[ 1] + ptr_0[ 1])) / (8 * reslution_x);
				slope_y = ((ptr_2[ 1] + 2*ptr_2[ 0] + ptr_2[-1]) - (ptr_0[ 1] + 2*ptr_0[ 0] + ptr_0[-1])) / (8 * reslution_y);

				aspect = atan(slope_y/slope_x)*AUGE_RADIAN_TO_DEGREE;
				if(slope_x>0)
				{
					aspect += 270.0;
				}
				else
				{
					aspect += 90.0;
				}
				aspect = 360.0 - aspect;
				*ptr = aspect;
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

	RESULTCODE DemAspectProcessorImpl::Aspect_Short(RasterBand* pinBand, RasterBand* poutBand)
	{
		g_uint width = pinBand->GetWidth();
		g_uint height= pinBand->GetHeight();

		short  v_h=0, v_v=0;
		short* input = (short*)pinBand->GetData();
		short* ptr_0 = input + 1; 
		short* ptr_1 = ptr_0 + width;
		short* ptr_2 = ptr_1 + width;
		short  v_min,v_max;
		double slope_x, slope_y;
		double aspect;
		double radian;
		double reslution_x = fabs(pinBand->GetResolution_X());
		double reslution_y = fabs(pinBand->GetResolution_Y());
		
		double a1 = atan(1.0)*AUGE_RADIAN_TO_DEGREE;
		double a2 = atan(-1.0)*AUGE_RADIAN_TO_DEGREE;

		g_int64 size = width*height*sizeof(short);
		double* output = (double*)malloc(size*sizeof(double));
		memset(output, 0, size*sizeof(double));

		g_int sum = 0;
		double* ptr = output + width + 1;
		for(g_uint i=1; i<height-1; i++)
		{	
			for(g_uint j=1; j<width-1; j++,ptr_0++,ptr_1++,ptr_2++,ptr++)
			{
				//slope_x = (ptr_1[-1] - ptr_1[1]) / (2 * reslution_x);
				//slope_y = (ptr_2[ 0] - ptr_0[0]) / (2 * reslution_y);

				slope_x = ((ptr_2[-1] + 2*ptr_1[-1] + ptr_0[-1]) - (ptr_2[ 1] + 2*ptr_1[ 1] + ptr_0[ 1])) / (8 * reslution_x);
				slope_y = ((ptr_2[ 1] + 2*ptr_2[ 0] + ptr_2[-1]) - (ptr_0[ 1] + 2*ptr_0[ 0] + ptr_0[-1])) / (8 * reslution_y);

				aspect = atan(slope_y/slope_x)*AUGE_RADIAN_TO_DEGREE;
				if(slope_x>0)
				{
					aspect += 270.0;
				}
				else
				{
					aspect += 90.0;
				}
				aspect = 360.0 - aspect;
				*ptr = aspect;
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


	//RESULTCODE DemAspectProcessorImpl::Aspect_Short(RasterBand* pinBand, RasterBand* poutBand)
	//{
	//	g_uint width = pinBand->GetWidth();
	//	g_uint height= pinBand->GetHeight();

	//	short  v_h=0, v_v=0;
	//	short* input = (short*)pinBand->GetData();
	//	short* ptr_0 = input + 1; 
	//	short* ptr_1 = ptr_0 + width;
	//	short* ptr_2 = ptr_1 + width;
	//	short  v_min,v_max;
	//	double slope_x, slope_y;
	//	double aspect;
	//	double radian;
	//	double reslution_x = pinBand->GetResolution_X();
	//	double reslution_y = pinBand->GetResolution_Y();

	//	g_int64 size = width*height*sizeof(short);
	//	short* output = (short*)malloc(size);
	//	memset(output, 0, size);
	//	
	//	g_int sum = 0;
	//	short* ptr = output + width + 1;
	//	for(g_uint i=1; i<height-1; i++)
	//	{	
	//		for(g_uint j=1; j<width-1; j++,ptr_0++,ptr_1++,ptr_2++,ptr++)
	//		{
	//			slope_x = ((ptr_2[-1] + 2*ptr_1[-1] + ptr_0[-1]) - (ptr_2[ 1] + 2*ptr_1[ 1] + ptr_0[ 1])) / (8 * reslution_x);
	//			slope_y = ((ptr_2[ 1] + 2*ptr_2[ 0] + ptr_2[-1]) - (ptr_0[ 1] + 2*ptr_0[ 0] + ptr_0[-1])) / (8 * reslution_y);

	//			if(fabs(slope_x)<EPSLN||fabs(slope_y)<EPSLN)
	//			{
	//				aspect = 0;
	//			}
	//			else
	//			{
	//				aspect =atan2(slope_y, -slope_x) / AUGE_RADIAN_TO_DEGREE; 
	//				if(aspect<0)
	//				{
	//					//aspect += 360.0;
	//				}
	//			}
	//			*ptr = aspect;
	//		}
	//		ptr_0 += 2;
	//		ptr_1 += 2;
	//		ptr_2 += 2;
	//		ptr  += 2;
	//	
	//	}
	//	
	//	poutBand->SetData(output);
	//	free(output);

	//	return AG_SUCCESS;
	//}


	RESULTCODE DemAspectProcessorImpl::Aspect_Double(RasterBand* pinBand, RasterBand* poutBand)
	{
		g_uint width = pinBand->GetWidth();
		g_uint height= pinBand->GetHeight();

		double  v_h=0, v_v=0;
		double* input = (double*)pinBand->GetData();
		double* ptr_0 = input + 1; 
		double* ptr_1 = ptr_0 + width;
		double* ptr_2 = ptr_1 + width;
		double  v_min,v_max;
		double slope_x, slope_y;
		double aspect;
		double value;
		double reslution_x = pinBand->GetResolution_X();
		double reslution_y = pinBand->GetResolution_Y();

		g_int64 size = width*height*sizeof(double);
		double* output = (double*)malloc(size);
		memset(output, 0, size);

		g_int sum = 0;
		double* ptr = output + width + 1;
		for(g_uint i=1; i<height-1; i++)
		{	
			for(g_uint j=1; j<width-1; j++,ptr_0++,ptr_1++,ptr_2++,ptr++)
			{
				slope_x = ((ptr_2[-1] + 2*ptr_1[-1] + ptr_0[-1]) - (ptr_2[ 1] + 2*ptr_1[ 1] + ptr_0[ 1])) / (8 * reslution_x);
				slope_y = ((ptr_2[ 1] + 2*ptr_2[ 0] + ptr_2[-1]) - (ptr_0[ 1] + 2*ptr_0[ 0] + ptr_0[-1])) / (8 * reslution_y);

				aspect = atan(slope_y/slope_x)*AUGE_RADIAN_TO_DEGREE;
				if(slope_x>0)
				{
					aspect += 270.0;
				}
				else
				{
					aspect += 90.0;
				}
				aspect = 360.0 - aspect;
				*ptr = aspect;
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
