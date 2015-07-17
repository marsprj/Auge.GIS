#include "RasterFormatConvertToJPEGProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeUser.h"

#include <fstream>

namespace auge
{
	RasterFormatConvertToJPEGProcessorImpl::RasterFormatConvertToJPEGProcessorImpl()
	{
		m_red = 0;
		m_green = 0;
		m_blue = 0;

		m_pUser = NULL;
	}

	RasterFormatConvertToJPEGProcessorImpl::~RasterFormatConvertToJPEGProcessorImpl()
	{

	}

	void RasterFormatConvertToJPEGProcessorImpl::SetInputRectangle(GEnvelope& rect)
	{
		m_rect = rect;
	}

	void RasterFormatConvertToJPEGProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void RasterFormatConvertToJPEGProcessorImpl::SetInputRaster(const char* rasterName)
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

	void RasterFormatConvertToJPEGProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void RasterFormatConvertToJPEGProcessorImpl::SetOutputRaster(const char* rasterName)
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

	void RasterFormatConvertToJPEGProcessorImpl::SetOutputPath(const char* path)
	{
		if(path==NULL)
		{
			m_out_path.clear();
		}
		else
		{
			m_out_path = path;
		}
	}

	void RasterFormatConvertToJPEGProcessorImpl::SetRed(g_uint i)
	{
		m_red = i;
	}

	void RasterFormatConvertToJPEGProcessorImpl::SetGreen(g_uint i)
	{
		m_green = i;
	}

	void RasterFormatConvertToJPEGProcessorImpl::SetBlue(g_uint i)
	{
		m_blue = i;
	}

	const char*	RasterFormatConvertToJPEGProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	RasterFormatConvertToJPEGProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	RasterFormatConvertToJPEGProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterFormatConvertToJPEGProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	const char*	RasterFormatConvertToJPEGProcessorImpl::GetOutputPath()
	{
		return m_out_path.empty() ? NULL : m_out_path.c_str();
	}

	RESULTCODE RasterFormatConvertToJPEGProcessorImpl::Execute()
	{
		GEnvelope&  inRect = m_rect;
		const char* inSourceName = GetInputDataSource();
		const char* inRasterName = GetInputRaster();
		const char* outSourceName = GetOutputDataSource();
		const char* outRasterName = GetOutputRaster();
		const char* outPath = GetOutputPath();

		Workspace* pWorkspace = NULL;
		RasterWorkspace* pinRasterWorkspace = NULL;
		RasterWorkspace* poutRasterWorkspace = NULL;

		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = pConnManager->GetWorkspace(m_pUser->GetID(), inSourceName);
		if(pWorkspace==NULL)
		{
			return AG_FAILURE;
		}
		pinRasterWorkspace = dynamic_cast<RasterWorkspace*>(m_pUser->GetID(), pWorkspace);

		Raster* pinRaster = NULL;
		RasterDataset* pinRasterDataset = NULL;
		pinRasterDataset = pinRasterWorkspace->OpenRasterDataset(inRasterName);
		if(pinRasterDataset==NULL)
		{
			return AG_FAILURE;
		}
		pinRaster = pinRasterDataset->GetRaster();

		// recalculate the requested extent, in case of the input rect is out of the original extent
		GEnvelope extent;
		GEnvelope& raster_extent = pinRaster->GetExtent();
		if(inRect.IsValid())
		{
			extent = raster_extent.Intersect(inRect);
		}
		else
		{
			extent = raster_extent;
		}

		// convert map extent to image rect
		GRect rastertRect;
		pinRaster->GetRasterRect(rastertRect, extent);

		RESULTCODE rc = AG_SUCCESS;
		Raster* poutRaster = NULL;
		RasterDataset* poutRasterDataset = NULL;
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();

		// Create output JPG Raster
		poutRaster = pRasterFactory->CreateRasterJPG(outRasterName, rastertRect.GetWidth(), rastertRect.GetHeight());
		if(poutRaster==NULL)
		{
			pinRasterDataset->Release();
			return AG_FAILURE;
		}

		Fill(poutRaster, pinRaster, rastertRect);

		if(outPath!=NULL)
		{				
			rc = poutRaster->Save(outPath, "jpeg");
			poutRaster->Release();
			pinRasterDataset->Release();
		}
		else
		{
			pWorkspace = pConnManager->GetWorkspace(m_pUser->GetID(), outSourceName);
			if(pWorkspace==NULL)
			{
				return AG_FAILURE;
			}
			poutRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

			rc = poutRasterWorkspace->AddRaster(poutRaster);

			poutRaster->Release();
			pinRasterDataset->Release();
		}

		return rc;
	}

	void RasterFormatConvertToJPEGProcessorImpl::Release()
	{
		delete this;
	}

	RESULTCODE RasterFormatConvertToJPEGProcessorImpl::Fill(Raster* poutRaster, Raster* pinRaster, GRect rastertRect)
	{
		g_uint band_count = pinRaster->GetBandCount();

		if(band_count==1)
		{
			Fill_1(poutRaster, pinRaster, rastertRect);
		}
		else if(band_count==2)
		{
			Fill_2(poutRaster, pinRaster, rastertRect);
		}
		else if(band_count==3)
		{
			Fill_3(poutRaster, pinRaster, rastertRect);
		}
		else
		{
			Fill_4(poutRaster, pinRaster, rastertRect);
		}

		return AG_SUCCESS;
	}

	RESULTCODE RasterFormatConvertToJPEGProcessorImpl::Fill_1(Raster* poutRaster, Raster* pinRaster, GRect rastertRect)
	{
		RasterBand* pinBand = pinRaster->GetBand(0);
		augePixelType type = pinRaster->GetPixelType();

		switch(type)
		{
		case augePixelByte:
			{

			}
			break;
		case augePixelUInt16:
		case augePixelInt16:
			Fill_Int16(poutRaster, pinBand, rastertRect);
			break;
		case augePixelUInt32:
		case augePixelInt32:
			break;
		case augePixelFloat32:
			break;
		case augePixelDouble:
			break;
		}

		return AG_SUCCESS;
	}

	RESULTCODE RasterFormatConvertToJPEGProcessorImpl::Fill_2(Raster* poutRaster, Raster* pinRaster, GRect rastertRect)
	{
		return AG_SUCCESS;
	}

	RESULTCODE RasterFormatConvertToJPEGProcessorImpl::Fill_3(Raster* poutRaster, Raster* pinRaster, GRect rastertRect)
	{
		if(m_red==0&&m_green==0&&m_blue==0)
		{
			m_red = 1;
			m_green=2;
			m_blue= 3;
		}

		RasterBand* pinBand = NULL;
		RasterBand* poutBand = NULL;

		pinBand  = pinRaster->GetBand(m_red-1);
		poutBand = poutRaster->GetBand(0);
		Fill(poutBand, pinBand, rastertRect);

		pinBand  = pinRaster->GetBand(m_green-1);
		poutBand = poutRaster->GetBand(1);
		Fill(poutBand, pinBand, rastertRect);

		pinBand  = pinRaster->GetBand(m_blue-1);
		poutBand = poutRaster->GetBand(2);
		Fill(poutBand, pinBand, rastertRect);

		return AG_SUCCESS;
	}

	RESULTCODE RasterFormatConvertToJPEGProcessorImpl::Fill_4(Raster* poutRaster, Raster* pinRaster, GRect rastertRect)
	{
		return AG_SUCCESS;
	}

	RESULTCODE RasterFormatConvertToJPEGProcessorImpl::Fill_Int16(Raster* poutRaster, RasterBand* pinBand, GRect rastertRect)
	{
		g_uint w = rastertRect.GetWidth();
		g_uint h = rastertRect.GetHeight();

		g_uint64 i_size = ((g_uint64)w) * h * pinBand->GetPixelSize();
		short* i_data = (short*)malloc(i_size);
		memset(i_data, 0, i_size);

		RESULTCODE rc = AG_SUCCESS;
		rc = pinBand->GetData(i_data, rastertRect);
		if(rc!=AG_SUCCESS)
		{
			free(i_data);
			return rc;
		}

		short i_min = AUGE_INT_MAX;
		short i_max = AUGE_INT_MIN;
		GetMinMax(i_min, i_max, i_data, w, h);
		short i_span = i_max - i_min;

		g_uint64 o_size = ((g_uint64)w) * h * sizeof(g_uchar);
		g_uchar* o_data = (g_uchar*)malloc(o_size);
		memset(o_data, 0, o_size);

		short	*i_p = i_data;
		g_uchar	*o_p = o_data;
		
		//std::ofstream os("g:\\temp\\aaabbb.txt");
		for(int i=0; i<h; i++)
		{
			for(int j=0; j<w; j++, i_p++, o_p++)
			{
				*o_p = 255.0f * ((*i_p) - i_min) / i_span;
				//os<<(int)(*o_p)<<"\t";
			}
			//os<<"\n";
			//os.flush();
		}
		//os.close();

		RasterBand* poutBand = poutRaster->GetBand(0);
		poutBand->SetData(o_data);
		poutBand = poutRaster->GetBand(1);
		poutBand->SetData(o_data);
		poutBand = poutRaster->GetBand(2);
		poutBand->SetData(o_data);

		free(i_data);
		free(o_data);

		return AG_SUCCESS;
	}

	void RasterFormatConvertToJPEGProcessorImpl::GetMinMax(short& i_min, short& i_max, short* i_data, int w, int h)
	{
		short* ptr = i_data;
		for(int i=0; i<h; i++)
		{
			for(int j=0; j<w; j++, ptr++)
			{
				i_min = i_min < *ptr ? i_min : *ptr;
				i_max = i_max > *ptr ? i_max : *ptr;
			}
		}
	}
	
	RESULTCODE RasterFormatConvertToJPEGProcessorImpl::Fill(RasterBand* poutBand, RasterBand* pinBand, GRect rastertRect)
	{
		if(pinBand==NULL)
		{
			g_int64 size = rastertRect.GetWidth() * rastertRect.GetHeight();
			g_uchar* data = (g_uchar*)malloc(size);
			memset(data, 0, size);
			poutBand->SetData(data);
			free(data);
		}
		else
		{
			augePixelType type = pinBand->GetPixelType();
			switch(type)
			{
			case augePixelByte:
				Fill_Byte(poutBand, pinBand, rastertRect);
				break;
			case augePixelUInt16:
			case augePixelInt16:
				Fill_Int16(poutBand, pinBand, rastertRect);
				break;
			case augePixelUInt32:
			case augePixelInt32:
				break;
			case augePixelFloat32:
				break;
			case augePixelDouble:
				break;
			}
		}

		return AG_SUCCESS;
	}

	RESULTCODE RasterFormatConvertToJPEGProcessorImpl::Fill_Byte(RasterBand* poutBand, RasterBand* pinBand, GRect rastertRect)
	{
		g_int64 size = rastertRect.GetWidth() * rastertRect.GetHeight();
		g_uchar* data = (g_uchar*)malloc(size);
		memset(data, 0, size);
		pinBand->GetData(data, rastertRect);
		poutBand->SetData(data);
		free(data);

		return AG_SUCCESS;
	}

	RESULTCODE RasterFormatConvertToJPEGProcessorImpl::Fill_Int16(RasterBand* poutBand, RasterBand* pinBand, GRect rastertRect)
	{
		return AG_SUCCESS;
	}

	void RasterFormatConvertToJPEGProcessorImpl::SetUser(User* pUser)
	{
		m_pUser = pUser;
	}
}
