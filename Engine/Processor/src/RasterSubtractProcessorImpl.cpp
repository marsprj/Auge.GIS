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
		m_in_raster_path_1 = "/";
		m_in_raster_path_2 = "/";
		m_out_raster_path = "/";
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

	void RasterSubtractProcessorImpl::SetInputPath_1(const char* rasterPath)
	{
		if(rasterPath==NULL)
		{
			m_in_raster_path_1 = "/";
		}
		else
		{
			if(strlen(rasterPath)==0)
			{
				m_in_raster_path_1 = "/";
			}
			else
			{
				m_in_raster_path_1 = rasterPath;
			}
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

	void RasterSubtractProcessorImpl::SetInputPath_2(const char* rasterPath)
	{
		if(rasterPath==NULL)
		{
			m_in_raster_path_2 = "/";
		}
		else
		{
			if(strlen(rasterPath)==0)
			{
				m_in_raster_path_2 = "/";
			}
			else
			{
				m_in_raster_path_2 = rasterPath;
			}
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

	void RasterSubtractProcessorImpl::SetOutputPath(const char* rasterPath)
	{
		if(rasterPath==NULL)
		{
			m_out_raster_path = "/";
		}
		else
		{
			if(strlen(rasterPath)==0)
			{
				m_out_raster_path = "/";
			}
			else
			{
				m_out_raster_path = rasterPath;
			}
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

	const char* RasterSubtractProcessorImpl::GetInputPath_1()
	{
		return m_in_raster_path_1.empty() ? NULL : m_in_raster_path_1.c_str();
	}

	const char*	RasterSubtractProcessorImpl::GetInputDataSource_2()
	{
		return m_in_source_name_2.empty() ? NULL : m_in_source_name_2.c_str();
	}

	const char*	RasterSubtractProcessorImpl::GetInputRaster_2()
	{
		return m_in_raster_name_2.empty() ? NULL : m_in_raster_name_2.c_str();
	}

	const char* RasterSubtractProcessorImpl::GetInputPath_2()
	{
		return m_in_raster_path_2.empty() ? NULL : m_in_raster_path_2.c_str();
	}

	const char*	RasterSubtractProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterSubtractProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}
	
	const char* RasterSubtractProcessorImpl::GetOutputPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	RESULTCODE RasterSubtractProcessorImpl::Execute()
	{
		const char* inSourceName_1 = GetInputDataSource_1();
		const char* inRasterName_1 = GetInputRaster_1();
		const char* inRasterPath_1 = GetInputPath_1();
		const char* inSourceName_2 = GetInputDataSource_2();
		const char* inRasterName_2 = GetInputRaster_2();
		const char* inRasterPath_2 = GetInputPath_2();
		const char* outSourceName = GetOutputDataSource();
		const char* outRasterName = GetOutputRaster();
		const char* outRasterPath = GetOutputPath();

		Workspace* pWorkspace = NULL;
		RasterWorkspace* pinRasterWorkspace_1 = NULL;
		RasterWorkspace* pinRasterWorkspace_2 = NULL;
		RasterWorkspace* poutRasterWorkspace = NULL;

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = pConnManager->GetWorkspace(m_user, inSourceName_1);
		if(pWorkspace==NULL)
		{
			char msg[AUGE_PATH_MAX];
			g_sprintf(msg, "Cannot get inputSource_1 [%s].", inSourceName_1);
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			return AG_FAILURE;
		}
		pinRasterWorkspace_1 = dynamic_cast<RasterWorkspace*>(pWorkspace);

		pWorkspace = pConnManager->GetWorkspace(m_user, inSourceName_2);
		if(pWorkspace==NULL)
		{
			char msg[AUGE_PATH_MAX];
			g_sprintf(msg, "Cannot get inputSource_2 [%s].", inSourceName_2);
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}
		pinRasterWorkspace_2 = dynamic_cast<RasterWorkspace*>(pWorkspace);

		pWorkspace = pConnManager->GetWorkspace(m_user, outSourceName);
		if(pWorkspace==NULL)
		{
			char msg[AUGE_PATH_MAX];
			g_sprintf(msg, "Cannot get outputSource [%s].", outSourceName);
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}
		poutRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

		Raster* pinRaster_1 = NULL;
		RasterFolder*  pinFolder_1 = NULL;
		//RasterDataset* pinRasterDataset_1 = NULL;

		Raster* pinRaster_2 = NULL;
		RasterFolder*  pinFolder_2 = NULL;
		//RasterDataset* pinRasterDataset_2 = NULL;

		Raster* poutRaster = NULL;
		RasterDataset* poutRasterDataset = NULL;

		// Get Raster 1
		pinFolder_1 = pinRasterWorkspace_1->GetFolder(inRasterPath_1);
		if(pinFolder_1==NULL)
		{
			char msg[AUGE_PATH_MAX];
			g_sprintf(msg, "inputPath_1 [%s] does not exist.", inRasterPath_1);
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			return AG_FAILURE;
		}
		pinRaster_1 = pinFolder_1->GetRasterDataset()->GetRaster(inRasterName_1);
		if(pinRaster_1==NULL)
		{
			char msg[AUGE_PATH_MAX];
			g_sprintf(msg, "inRasterName_1 [%s] does not exist.", inRasterName_1);
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			pinFolder_1->Release();
			return AG_FAILURE;
		}

		// Get Raster 2
		pinFolder_2 = pinRasterWorkspace_2->GetFolder(inRasterPath_2);
		if(pinFolder_2==NULL)
		{
			char msg[AUGE_PATH_MAX];
			g_sprintf(msg, "inputPath_2 [%s] does not exist.", inRasterPath_2);
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			pinRaster_1->Release();
			pinFolder_1->Release();
			return AG_FAILURE;
		}
		pinRaster_2 = pinFolder_2->GetRasterDataset()->GetRaster(inRasterName_2);
		if(pinRaster_2==NULL)
		{
			char msg[AUGE_PATH_MAX];
			g_sprintf(msg, "inRasterName_2 [%s] does not exist.", inRasterName_2);
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			pinRaster_1->Release();
			pinFolder_1->Release();
			pinFolder_2->Release();
			return AG_FAILURE;
		}

		g_uint bands_1 = pinRaster_1->GetBandCount();
		g_uint bands_2 = pinRaster_2->GetBandCount();
		if(bands_1!=bands_2)
		{
			GError* pError = augeGetErrorInstance();
			pError->SetError("Tow Raster should have same bands");

			pinRaster_1->Release();
			pinRaster_2->Release();
			pinFolder_1->Release();
			pinFolder_2->Release();
			return AG_FAILURE;
		}

		g_uint width_1 = pinRaster_1->GetWidth();
		g_uint width_2 = pinRaster_2->GetWidth();
		if(width_1 != width_2)
		{
			GError* pError = augeGetErrorInstance();
			pError->SetError("Tow Raster should have same size");

			pinRaster_1->Release();
			pinRaster_2->Release();
			pinFolder_1->Release();
			pinFolder_2->Release();
			return AG_FAILURE;
		}

		g_uint height_1 = pinRaster_1->GetHeight();
		g_uint height_2 = pinRaster_2->GetHeight();
		if(height_1 != height_1)
		{
			GError* pError = augeGetErrorInstance();
			pError->SetError("Tow Raster should have same size");

			pinRaster_1->Release();
			pinRaster_2->Release();
			pinFolder_1->Release();
			pinFolder_2->Release();
			return AG_FAILURE;
		}

		poutRaster = Subtract(pinRaster_1, pinRaster_2);

		if(poutRaster!=NULL)
		{
			poutRaster->SetName(outRasterName);
		}

		RasterFolder* poutFolder = poutRasterWorkspace->GetFolder(outRasterPath);
		if(poutFolder==NULL)
		{
			pinRaster_1->Release();
			pinRaster_2->Release();
			poutRaster->Release();
			pinFolder_1->Release();
			pinFolder_2->Release();
			return AG_FAILURE;
		}
		RESULTCODE rc = poutFolder->GetRasterDataset()->AddRaster(outRasterName, poutRaster);

		pinRaster_1->Release();
		pinRaster_2->Release();
		poutRaster->Release();
		poutFolder->Release();
		pinFolder_1->Release();
		pinFolder_2->Release();

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
