#include "RasterThumbnailProcessorImpl.h"
#include "AugeRaster.h"

namespace auge
{
	RasterThumbnailProcessorImpl::RasterThumbnailProcessorImpl()
	{
		m_red = 0;
		m_green = 0;
		m_blue = 0;
	}

	RasterThumbnailProcessorImpl::~RasterThumbnailProcessorImpl()
	{

	}

	void RasterThumbnailProcessorImpl::SetInputRaster(const char* rasterPath)
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

	void RasterThumbnailProcessorImpl::SetOutputRaster(const char* rasterPath)
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

	const char*	RasterThumbnailProcessorImpl::GetInputRaster()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char*	RasterThumbnailProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	RESULTCODE RasterThumbnailProcessorImpl::Execute()
	{
		const char* inRasterPath = GetInputRaster();
		const char* outRasterPath = GetOutputRaster();

		if(inRasterPath==NULL)
		{
			return AG_FAILURE;
		}
		if(outRasterPath==NULL)
		{
			return AG_FAILURE;
		}

		RasterIO* pRasterIO = augeGetRasterIOInstance();
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();

		Raster* pinRaster = NULL;
		Raster* poutRaster = NULL;

		pinRaster = pRasterIO->Read(inRasterPath);
		if(pinRaster==NULL)
		{
			return AG_FAILURE;
		}

		g_uint r_w = pinRaster->GetWidth();
		g_uint r_h= pinRaster->GetHeight();

		float scale = GetScale(r_w, r_h);
		g_uint t_w = r_w * scale;
		g_uint t_h = r_h * scale;

		m_step = (float)r_w / t_w;

		// Create output JPG Raster
		poutRaster = pRasterFactory->CreateRasterJPG("", t_w, t_h);
		if(poutRaster==NULL)
		{
			pinRaster->Release();
			return AG_FAILURE;
		}

		Fill(poutRaster, pinRaster);

		poutRaster->Save(outRasterPath);
		poutRaster->Release();
		pinRaster->Release();
		
		return AG_SUCCESS;
	}

	void RasterThumbnailProcessorImpl::Release()
	{
		delete this;
	}

	void RasterThumbnailProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}

	RESULTCODE RasterThumbnailProcessorImpl::Fill(Raster* poutRaster, Raster* pinRaster)
	{
		g_uint band_count = pinRaster->GetBandCount();

		if(band_count==1)
		{
			Fill_1(poutRaster, pinRaster);
		}
		else if(band_count==2)
		{
			Fill_2(poutRaster, pinRaster);
		}
		else if(band_count==3)
		{
			Fill_3(poutRaster, pinRaster);
		}
		else
		{
			Fill_4(poutRaster, pinRaster);
		}

		return AG_SUCCESS;
	}

	RESULTCODE RasterThumbnailProcessorImpl::Fill_1(Raster* poutRaster, Raster* pinRaster)
	{
		RasterBand* pinBand = pinRaster->GetBand(0);
		augePixelType type = pinRaster->GetPixelType();

		switch(type)
		{
		case augePixelByte:
			break;
		case augePixelUInt16:
		case augePixelInt16:
			Fill_Int16(poutRaster, pinBand);
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

	RESULTCODE RasterThumbnailProcessorImpl::Fill_2(Raster* poutRaster, Raster* pinRaster)
	{
		return AG_SUCCESS;
	}

	RESULTCODE RasterThumbnailProcessorImpl::Fill_3(Raster* poutRaster, Raster* pinRaster)
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
		Fill(poutBand, pinBand);

		pinBand  = pinRaster->GetBand(m_green-1);
		poutBand = poutRaster->GetBand(1);
		Fill(poutBand, pinBand);

		pinBand  = pinRaster->GetBand(m_blue-1);
		poutBand = poutRaster->GetBand(2);
		Fill(poutBand, pinBand);

		return AG_SUCCESS;
	}

	RESULTCODE RasterThumbnailProcessorImpl::Fill_4(Raster* poutRaster, Raster* pinRaster)
	{
		return AG_SUCCESS;
	}
	RESULTCODE RasterThumbnailProcessorImpl::Fill_Int16(Raster* poutRaster, RasterBand* pinBand)
	{
		g_uint64 r_w = pinBand->GetWidth();
		g_uint64 r_h = pinBand->GetHeight();
		g_uint64 offset = 0;

		g_uint t_w = poutRaster->GetWidth();
		g_uint t_h = poutRaster->GetHeight();
		g_uint size = t_w * t_h * sizeof(g_byte);

		g_byte* t_data = (g_byte*)malloc(size);
		memset(t_data, 255, size);
		g_byte* t_p = t_data;

		short v = 0;
		short* r_data = (short*)pinBand->GetData();
		short i_min = AUGE_INT_MAX;
		short i_max = AUGE_INT_MIN;
		GetMinMax(i_min, i_max, r_data, r_w, r_h);
		short i_span = i_max - i_min;

		g_uint i=0, j=0;
		g_uint row=0, col=0;
		for(i=0; i<t_h; i++,row+=m_step)
		{
			row = row < r_h ? row : r_h-1;
			for(j=0,col=0; j<t_w; j++, col+=m_step)
			{
				col = col < r_w ? col : r_w-1;

				offset = row * r_w + col;
				v = *(r_data + offset);
				*(t_p++) = 255.0f * (v - i_min) / i_span;
				//*(t_p++) = 255.0f * (v) / AUGE_SHORT_MAX;
			}
		}

		/*g_uint w = pinBand->GetWidth();
		g_uint h = pinBand->GetHeight();

		RESULTCODE rc = AG_SUCCESS;
		short* i_data = (short*)pinBand->GetData();
		if(i_data==NULL)
		{	
		return AG_FAILURE;
		}

		short i_min = AUGE_INT_MAX;
		short i_max = AUGE_INT_MIN;
		GetMinMax(i_min, i_max, i_data, w, h);
		short i_span = i_max - i_min;

		g_uint64 o_size = ((g_uint64)w) * h * sizeof(g_byte);
		g_byte* o_data = (g_byte*)malloc(o_size);
		memset(o_data, 0, o_size);

		short	*i_p = i_data;
		g_byte	*o_p = o_data;

		for(int i=0; i<h; i++)
		{
		for(int j=0; j<w; j++, i_p++, o_p++)
		{
		*o_p = 255.0f * ((*i_p) - i_min) / i_span;
		}
		}*/

		RasterBand* poutBand = poutRaster->GetBand(0);
		poutBand->SetData(t_data);
		poutBand = poutRaster->GetBand(1);
		poutBand->SetData(t_data);
		poutBand = poutRaster->GetBand(2);
		poutBand->SetData(t_data);

		free(t_data);

		return AG_SUCCESS;
	}

	//RESULTCODE RasterThumbnailProcessorImpl::Fill_Int16(Raster* poutRaster, RasterBand* pinBand)
	//{
	//	g_uint w = pinBand->GetWidth();
	//	g_uint h = pinBand->GetHeight();

	//	RESULTCODE rc = AG_SUCCESS;
	//	short* i_data = (short*)pinBand->GetData();
	//	if(i_data==NULL)
	//	{	
	//		return AG_FAILURE;
	//	}

	//	short i_min = AUGE_INT_MAX;
	//	short i_max = AUGE_INT_MIN;
	//	GetMinMax(i_min, i_max, i_data, w, h);
	//	short i_span = i_max - i_min;

	//	g_uint64 o_size = ((g_uint64)w) * h * sizeof(g_byte);
	//	g_byte* o_data = (g_byte*)malloc(o_size);
	//	memset(o_data, 0, o_size);

	//	short	*i_p = i_data;
	//	g_byte	*o_p = o_data;

	//	for(int i=0; i<h; i++)
	//	{
	//		for(int j=0; j<w; j++, i_p++, o_p++)
	//		{
	//			*o_p = 255.0f * ((*i_p) - i_min) / i_span;
	//		}
	//	}
	//	
	//	RasterBand* poutBand = poutRaster->GetBand(0);
	//	poutBand->SetData(o_data);
	//	poutBand = poutRaster->GetBand(1);
	//	poutBand->SetData(o_data);
	//	poutBand = poutRaster->GetBand(2);
	//	poutBand->SetData(o_data);

	//	free(o_data);

	//	return AG_SUCCESS;
	//}

	void RasterThumbnailProcessorImpl::GetMinMax(short& i_min, short& i_max, short* i_data, int w, int h)
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

	RESULTCODE RasterThumbnailProcessorImpl::Fill(RasterBand* poutBand, RasterBand* pinBand)
	{
		if(pinBand==NULL)
		{
			g_uint64 width = pinBand->GetWidth();
			g_uint64 height= pinBand->GetHeight();

			g_int64 size = width * height;
			g_byte* data = (g_byte*)malloc(size);
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
				Fill_Byte(poutBand, pinBand);
				break;
			case augePixelUInt16:
			case augePixelInt16:
				Fill_Int16(poutBand, pinBand);
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

	RESULTCODE RasterThumbnailProcessorImpl::Fill_Byte(RasterBand* poutBand, RasterBand* pinBand)
	{
		g_uint64 r_w = pinBand->GetWidth();
		g_uint64 r_h = pinBand->GetHeight();
		g_uint64 offset = 0;

		g_uint t_w = poutBand->GetWidth();
		g_uint t_h = poutBand->GetHeight();
		g_uint size = t_w * t_h * sizeof(g_byte);

		g_byte* t_data = (g_byte*)malloc(size);
		memset(t_data, 255, size);
		g_byte* t_p = t_data;

		g_byte v = 0;
		g_byte* r_data = (g_byte*)pinBand->GetData();

		g_uint i=0, j=0;
		g_uint row=0, col=0;
		for(i=0; i<t_h; i++,row+=m_step)
		{
			row = row < r_h ? row : r_h-1;
			for(j=0,col=0; j<t_w; j++, col+=m_step)
			{
				col = col < r_w ? col : r_w-1;

				offset = row * r_w + col;
				v = *(r_data + offset);
				*(t_p++) = v;
			}
		}

		poutBand->SetData(t_data);
		free(t_data);
	
		return AG_SUCCESS;
	}

	RESULTCODE RasterThumbnailProcessorImpl::Fill_Int16(RasterBand* poutBand, RasterBand* pinBand)
	{
		return AG_SUCCESS;
	}

	float RasterThumbnailProcessorImpl::GetScale(g_uint w, g_uint h)
	{
		float scale = 0.0f;
		if(w>h)
			scale = AUGE_THUMBNAIL_SIZE / (float)w ;
		else
			scale = AUGE_THUMBNAIL_SIZE / (float)h;
		return scale;
	}
}