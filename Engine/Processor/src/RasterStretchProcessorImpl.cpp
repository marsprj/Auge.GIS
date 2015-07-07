#include "RasterStretchProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"

namespace auge
{
	RasterStretchProcessorImpl::RasterStretchProcessorImpl()
	{
		m_color_start = GColor(255,0,0,255);
		m_color_end = GColor(0,255,0,255);
	}

	RasterStretchProcessorImpl::~RasterStretchProcessorImpl()
	{

	}
	
	void RasterStretchProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void RasterStretchProcessorImpl::SetInputRaster(const char* rasterName)
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

	void RasterStretchProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void RasterStretchProcessorImpl::SetOutputRaster(const char* rasterName)
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

	void RasterStretchProcessorImpl::SetStartColor(GColor color)
	{
		m_color_start = color;
	}

	void RasterStretchProcessorImpl::SetEndColor(GColor color)
	{
		m_color_end = color;
	}

	const char*	RasterStretchProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	RasterStretchProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	RasterStretchProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterStretchProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	GColor&	RasterStretchProcessorImpl::GetStartColor()
	{
		return m_color_start;
	}

	GColor&	RasterStretchProcessorImpl::GetEndColor()
	{
		return m_color_end;
	}

	RESULTCODE RasterStretchProcessorImpl::Execute()
	{
		const char* inSourceName = GetInputDataSource();
		const char* inRasterName = GetInputRaster();
		const char* outSourceName = GetOutputDataSource();
		const char* outRasterName = GetOutputRaster();

		Workspace* pWorkspace = NULL;
		RasterWorkspace* pinRasterWorkspace = NULL;
		RasterWorkspace* poutRasterWorkspace = NULL;

		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = pConnManager->GetWorkspace(inSourceName);
		if(pWorkspace==NULL)
		{
			return AG_FAILURE;
		}
		pinRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

		pWorkspace = pConnManager->GetWorkspace(outSourceName);
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
		
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();

		poutRaster = pRasterFactory->CreateRasterPNG(outRasterName, pinRaster->GetWidth(), pinRaster->GetHeight());
		if(poutRaster==NULL)
		{
			pinRasterDataset->Release();
			return AG_FAILURE;
		}

		if(!Stretch(pinRaster, poutRaster))
		{
			pinRasterDataset->Release();
			poutRaster->Release();
			return AG_FAILURE;
		}

		RESULTCODE rc = poutRasterWorkspace->AddRaster(poutRaster);
		poutRaster->Release();
		pinRasterDataset->Release();

		return rc;

		return AG_SUCCESS;
	}

	void RasterStretchProcessorImpl::Release()
	{
		delete this;
	}

	bool RasterStretchProcessorImpl::Stretch(Raster* pinRaster, Raster* poutRaster)
	{
		RasterBand* pinBand = pinRaster->GetBand(0);

		RasterBand* poutBand_R = poutRaster->GetBand(0);
		RasterBand* poutBand_G = poutRaster->GetBand(1);
		RasterBand* poutBand_B = poutRaster->GetBand(2);
		RasterBand* poutBand_A = poutRaster->GetBand(3);

		size_t size = sizeof(short);

		size_t pixel_count = poutRaster->GetWidth() * poutRaster->GetHeight();
		size_t buffer_size = pixel_count * sizeof(g_uchar);
		g_uchar* r_b = (g_uchar*)malloc(buffer_size);
		g_uchar* g_b = (g_uchar*)malloc(buffer_size);
		g_uchar* b_b = (g_uchar*)malloc(buffer_size);
		memset(r_b, 0, buffer_size);
		memset(g_b, 0, buffer_size);
		memset(b_b, 0, buffer_size);

		short v_min=0, v_max=0;
		pinBand->GetMinMaxValue(v_min, v_max);
		short* data = (short*)pinBand->GetData();

		short value_span = v_max - v_min;
		g_uchar r_s = m_color_start.GetRed();
		g_uchar g_s = m_color_start.GetGreen();
		g_uchar b_s = m_color_start.GetBlue();
		g_uchar r_e = m_color_end.GetRed();
		g_uchar g_e = m_color_end.GetGreen();
		g_uchar b_e = m_color_end.GetBlue();

		float h_s=0.0f, s_s=0.0f, v_s=0.0f;
		float h_e=0.0f, s_e=0.0f, v_e=0.0f;
		
		auge_rgb_2_hsv(r_s, g_s, b_s, h_s, s_s, v_s);
		auge_rgb_2_hsv(r_e, g_e, b_e, h_e, s_e, v_e);
		
		float h_span = h_e - h_s;
		float s_span = s_e - s_s;
		float v_span = v_e - v_s;
		float scale = 1.0f;

		float h=0.0f, s=0.0f, v=0.0f;
		g_uchar r=0, g=0, b=0;

		data[0] = 1000;
		data[1] = 3000;
		short value = 0;
		for(int i=0; i<pixel_count; i++)
		{
			value = data[i];
			scale = ((float)value - (float)v_min) / (float)value_span;

			h = h_span * scale + h_s;
			s = s_span * scale + s_s;
			v = v_span * scale + v_s;

			auge_hsv_2_rgb(h, s, v, r, g, b);
			r_b[i] = r;
			g_b[i] = g;
			b_b[i] = b;

			//printf("[%d]:\t[%f\t%f\t%f]\t[%d\t%d\t%d]\n",value, h, s, v, r,g,b);
		}

		poutBand_R->SetData(r_b);
		poutBand_G->SetData(g_b);
		poutBand_B->SetData(b_b);

		memset(r_b, 255, buffer_size);
		poutBand_A->SetData(r_b);

		free(r_b);
		free(g_b);
		free(b_b);

		return true;
	}
}
