#include "RasterThresholdProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeUser.h"

#include <math.h>
#include <fstream>

namespace auge
{
	void auge_graylize_byte(g_byte* gray_data, g_uint64 size, Raster* pinRaster);
		
	RasterThresholdProcessorImpl::RasterThresholdProcessorImpl()
	{
		m_user = 0;
		m_in_raster_path = "/";
		m_out_raster_path = "/";
		m_thresholder = augeThresholderAverage;
	}

	RasterThresholdProcessorImpl::~RasterThresholdProcessorImpl()
	{

	}
	
	void RasterThresholdProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void RasterThresholdProcessorImpl::SetInputRaster(const char* rasterName)
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

	void RasterThresholdProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void RasterThresholdProcessorImpl::SetOutputRaster(const char* rasterName)
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
	
	void RasterThresholdProcessorImpl::SetThresholder(augeRasterThresholder thresholder)
	{
		m_thresholder = thresholder;
	}

	const char*	RasterThresholdProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	RasterThresholdProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	RasterThresholdProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterThresholdProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	void RasterThresholdProcessorImpl::SetInputPath(const char* rasterPath)
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

	void RasterThresholdProcessorImpl::SetOutputPath(const char* rasterPath)
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

	const char* RasterThresholdProcessorImpl::GetInputRasterPath()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char* RasterThresholdProcessorImpl::GetOutputRasterPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	RESULTCODE RasterThresholdProcessorImpl::Execute()
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
			const char* msg = "无法连接到数据源";
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
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
		g_uint band_count = pinRaster->GetBandCount();
		if(band_count<3||band_count>4)
		{
			const char* msg = "二值化仅支持3波段彩色栅格数据";
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			pinFolder->Release();
			return AG_FAILURE;
		}

		augePixelType pixelType = pinRaster->GetPixelType();
		if(pixelType!=augePixelByte)
		{
			const char* msg = "二值化仅支持8位像素栅格数据";
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			pinFolder->Release();
			return AG_FAILURE;
		}
		
		switch(m_thresholder)
		{
		case augeThresholderAverage:
			poutRaster = Threshold_Average(pinRaster);
			break;		}
		
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

	void RasterThresholdProcessorImpl::Release()
	{
		delete this;
	}


	void RasterThresholdProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}
	
	Raster* RasterThresholdProcessorImpl::Threshold_Average(Raster* pinRaster)
	{
		Raster* poutRaster = NULL;
		
		RESULTCODE rc = AG_SUCCESS;
		augePixelType type = pinRaster->GetPixelType();

		switch(type)
		{
		case augePixelByte:
			poutRaster = Threshold_Average_Byte(pinRaster);
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

		return poutRaster;
	}

	Raster*	RasterThresholdProcessorImpl::Threshold_Average_Byte(Raster* pinRaster)
	{	
		g_uint band_count = pinRaster->GetBandCount();
		g_uint64 w = pinRaster->GetWidth();
		g_uint64 h = pinRaster->GetHeight();
		g_uint64 size = w * h * sizeof(g_byte);
		g_byte* gray_data = (g_byte*)malloc(size);
		memset(gray_data, 0, size);
		auge_graylize_byte(gray_data, size, pinRaster);

		g_byte average = 0;
		average = auge_average_byte(gray_data, size);

		g_byte* ptr = gray_data;
		for(g_uint64 i=0; i<size; i++, ptr++)
		{
			*ptr = *ptr < average ? 0 : 255;
		}
		
		pinRaster->GetBand(0)->SetData(gray_data);
		pinRaster->GetBand(1)->SetData(gray_data);
		pinRaster->GetBand(2)->SetData(gray_data);

		free(gray_data);

		pinRaster->AddRef();
		return pinRaster;
	}

	//Raster*	RasterThresholdProcessorImpl::Threshold_Average_Int16(Raster* pinRaster)
	//{	
	//	g_uint band_count = pinRaster->GetBandCount();
	//	g_uint64 w = pinRaster->GetWidth();
	//	g_uint64 h = pinRaster->GetHeight();
	//	g_uint64 size = w * h * sizeof(g_int16);
	//	g_int16* gray_data = (g_int16*)malloc(size);
	//	memset(gray_data, 0, size);
	//	auge_graylize_byte(gray_data, size, pinRaster);

	//	g_byte average = 0;
	//	//average = auge_average_int16(gray_data, size);

	//	g_byte* ptr = gray_data;
	//	for(g_uint64 i=0; i<size; i++, ptr++)
	//	{
	//		*ptr = *ptr < average ? 0 : 255;
	//	}

	//	pinRaster->GetBand(0)->SetData(gray_data);
	//	pinRaster->GetBand(1)->SetData(gray_data);
	//	pinRaster->GetBand(2)->SetData(gray_data);

	//	free(gray_data);

	//	pinRaster->AddRef();
	//	return pinRaster;

	//}

	void auge_graylize_byte(g_byte* gray_data, g_uint64 size, Raster* pRaster)
	{
		memset(gray_data, 0, size);
		RasterBand* prBand = pRaster->GetBand(0);
		RasterBand* pgBand = pRaster->GetBand(1);
		RasterBand* pbBand = pRaster->GetBand(2);

		g_byte* r = (g_byte*)(prBand->GetData());
		g_byte* g = (g_byte*)(pgBand->GetData());
		g_byte* b = (g_byte*)(pbBand->GetData());
		g_byte* ptr = gray_data;
		int r_c = 299;
		int g_c = 587;
		int b_c = 114;
		//Gray = R*0.299 + G*0.587 + B*0.114
		for(g_uint64 i=0; i<size; i++, r++, g++, b++, ptr++)
		{
			*ptr = ((*r)*r_c + (*g)*g_c + (*b)*b_c) / 1000;
		}
	}

}
