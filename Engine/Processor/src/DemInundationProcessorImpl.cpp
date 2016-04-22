#include "DemInundationProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeUser.h"

#include <math.h>

namespace auge
{
	DemInundationProcessorImpl::DemInundationProcessorImpl()
	{
		m_user = 0;
		m_in_raster_path = "/";
		m_out_raster_path = "/";
		m_height = AUGE_FLOAT_MIN;
	}

	DemInundationProcessorImpl::~DemInundationProcessorImpl()
	{

	}
	
	void DemInundationProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void DemInundationProcessorImpl::SetInputRaster(const char* rasterName)
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

	void DemInundationProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void DemInundationProcessorImpl::SetOutputRaster(const char* rasterName)
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

	const char*	DemInundationProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	DemInundationProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	DemInundationProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	DemInundationProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	void DemInundationProcessorImpl::SetInputPath(const char* rasterPath)
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

	void DemInundationProcessorImpl::SetOutputPath(const char* rasterPath)
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

	const char* DemInundationProcessorImpl::GetInputRasterPath()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char* DemInundationProcessorImpl::GetOutputRasterPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	void DemInundationProcessorImpl::SetHeight(float height)
	{
		m_height = height;
	}

	RESULTCODE DemInundationProcessorImpl::Execute()
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

		GError* pError = augeGetErrorInstance();
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
			const char* msg = "无法打开输入栅格数据";
			pError->SetError(msg);
			pinFolder->Release();
			return AG_FAILURE;
		}

		g_uint band_count = pinRaster->GetBandCount();
		if(band_count!=1)
		{
			const char* msg = "Dem坡向计算仅支持单波段Dem数据";
			pError->SetError(msg);
			pinFolder->Release();
			return AG_FAILURE;
		}
		
		poutRaster = Inundate(pinRaster, m_height);

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

	void DemInundationProcessorImpl::Release()
	{
		delete this;
	}

	void DemInundationProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}

	Raster* DemInundationProcessorImpl::Inundate(Raster* pinRaster, float height)
	{
		Raster* poutRaster = NULL;
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();
		augePixelType pixelType = pinRaster->GetPixelType();
		//poutRaster = pRasterFactory->CreateRaster("", augePixelDouble, 1, pinRaster->GetExtent(), pinRaster->GetWidth(), pinRaster->GetHeight(), pinRaster->GetSpatialReference());
		poutRaster = pRasterFactory->CreateRaster("", pixelType, 1, pinRaster->GetExtent(), pinRaster->GetWidth(), pinRaster->GetHeight(), pinRaster->GetSpatialReference());
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
				rc = Inundate_Byte(pinBand, poutBand, height);
				break;
			case augePixelUInt16:
			case augePixelInt16:
				Inundate_Short(pinBand, poutBand, height);
				break;
			case augePixelUInt32:
			case augePixelInt32:
				break;
			case augePixelFloat32:
				Inundate_Float(pinBand, poutBand, height);
				break;
			case augePixelDouble:
				Inundate_Double(pinBand, poutBand, height);
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

	RESULTCODE DemInundationProcessorImpl::Inundate_Byte(RasterBand* pinBand, RasterBand* poutBand, g_byte water_height)
	{
		g_uint width = pinBand->GetWidth();
		g_uint height= pinBand->GetHeight();

		g_byte* input = (g_byte*)pinBand->GetData();

		g_int64 size = width*height*sizeof(g_byte);
		g_byte* output = (g_byte*)malloc(size*sizeof(g_byte));
		memset(output, 0, size*sizeof(g_byte));

		g_byte* ptr_0 = input;
		g_byte* ptr_1 = output;
		for(g_uint i=0; i<height; i++)
		{	
			for(g_uint j=0; j<width; j++,ptr_0++, ptr_1++)
			{
				(*ptr_1) = (*ptr_0)>water_height ? (*ptr_0) : water_height;
			}
		}

		poutBand->SetData(output);
		free(output);

		return AG_SUCCESS;
	}

	RESULTCODE DemInundationProcessorImpl::Inundate_Short(RasterBand* pinBand, RasterBand* poutBand, g_int16 water_height)
	{
		g_uint width = pinBand->GetWidth();
		g_uint height= pinBand->GetHeight();

		short* input = (short*)pinBand->GetData();

		g_int64 size = width*height*sizeof(short);
		short* output = (short*)malloc(size*sizeof(short));
		memset(output, 0, size*sizeof(short));

		short* ptr_0 = input;
		short* ptr_1 = output;
		for(g_uint i=0; i<height; i++)
		{	
			for(g_uint j=0; j<width; j++,ptr_0++, ptr_1++)
			{
				(*ptr_1) = (*ptr_0)>water_height ? (*ptr_0) : water_height;
			}
		}

		poutBand->SetData(output);
		free(output);

		return AG_SUCCESS;
	}


	//RESULTCODE DemInundationProcessorImpl::InundateShort(RasterBand* pinBand, RasterBand* poutBand)
	//{
	//	g_uint width = pinBand->GetWidth();
	//	g_uint height= pinBand->GetHeight();

	//	short  v_h=0, v_v=0;
	//	short* input = (short*)pinBand->GetData();
	//	short* ptr_0 = input + 1; 
	//	short* ptr_1 = ptr_0 + width;
	//	short* ptr_2 = ptr_1 + width;
	//	short  v_min,v_max;
	//	float slope_x, slope_y;
	//	float aspect;
	//	float radian;
	//	float reslution_x = pinBand->GetResolution_X();
	//	float reslution_y = pinBand->GetResolution_Y();

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

	RESULTCODE DemInundationProcessorImpl::Inundate_Float(RasterBand* pinBand, RasterBand* poutBand, float water_height)
	{
		g_uint width = pinBand->GetWidth();
		g_uint height= pinBand->GetHeight();

		float* input = (float*)pinBand->GetData();

		g_int64 size = width*height*sizeof(float);
		float* output = (float*)malloc(size*sizeof(float));
		memset(output, 0, size*sizeof(float));

		float* ptr_0 = input;
		float* ptr_1 = output;
		for(g_uint i=0; i<height; i++)
		{	
			for(g_uint j=0; j<width; j++,ptr_0++, ptr_1++)
			{
				(*ptr_1) = (*ptr_0)>water_height ? (*ptr_0) : water_height;
			}
		}

		poutBand->SetData(output);
		free(output);

		return AG_SUCCESS;
	}


	RESULTCODE DemInundationProcessorImpl::Inundate_Double(RasterBand* pinBand, RasterBand* poutBand, double water_height)
	{
		g_uint width = pinBand->GetWidth();
		g_uint height= pinBand->GetHeight();

		double* input = (double*)pinBand->GetData();

		g_int64 size = width*height*sizeof(double);
		double* output = (double*)malloc(size*sizeof(double));
		memset(output, 0, size*sizeof(double));

		double* ptr_0 = input;
		double* ptr_1 = output;
		for(g_uint i=0; i<height; i++)
		{	
			for(g_uint j=0; j<width; j++,ptr_0++, ptr_1++)
			{
				(*ptr_1) = (*ptr_0)>water_height ? (*ptr_0) : water_height;
			}
		}

		poutBand->SetData(output);
		free(output);

		return AG_SUCCESS;
	}

	
}
