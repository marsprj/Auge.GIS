#include "RasterStretchProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeUser.h"

namespace auge
{
	RasterStretchProcessorImpl::RasterStretchProcessorImpl()
	{
		//m_color_start = GColor(255,0,0,255);
		//m_color_end = GColor(0,255,0,255);

		m_color_end = GColor(255,255,255,255);
		m_color_start = GColor(0,0,0,255);

		m_user = 0;
		m_in_raster_path = "/";
		m_out_raster_path = "/";
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

	void RasterStretchProcessorImpl::SetInputPath(const char* rasterPath)
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

	void RasterStretchProcessorImpl::SetOutputPath(const char* rasterPath)
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

	const char* RasterStretchProcessorImpl::GetInputRasterPath()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char* RasterStretchProcessorImpl::GetOutputRasterPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	GColor&	RasterStretchProcessorImpl::GetStartColor()
	{
		return m_color_start;
	}

	GColor&	RasterStretchProcessorImpl::GetEndColor()
	{
		return m_color_end;
	}

	//RESULTCODE RasterStretchProcessorImpl::Execute()
	//{
	//	const char* inSourceName = GetInputDataSource();
	//	const char* inRasterName = GetInputRaster();
	//	const char* inRasterPath = GetInputRasterPath();
	//	const char* outSourceName = GetOutputDataSource();
	//	const char* outRasterName = GetOutputRaster();
	//	const char* outRasterPath = GetOutputRasterPath();

	//	Workspace* pWorkspace = NULL;
	//	RasterWorkspace* pinRasterWorkspace = NULL;
	//	RasterWorkspace* poutRasterWorkspace = NULL;

	//	ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

	//	pWorkspace = pConnManager->GetWorkspace(m_user, inSourceName);
	//	if(pWorkspace==NULL)
	//	{
	//		return AG_FAILURE;
	//	}
	//	pinRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

	//	pWorkspace = pConnManager->GetWorkspace(m_user, outSourceName);
	//	if(pWorkspace==NULL)
	//	{
	//		return AG_FAILURE;
	//	}
	//	poutRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

	//	Raster* pinRaster = NULL;
	//	RasterFolder* pinFolder = NULL;

	//	Raster* poutRaster = NULL;
	//	RasterFolder* poutFolder = NULL;

	//	pinFolder = pinRasterWorkspace->GetFolder(inRasterPath);
	//	if(pinFolder==NULL)
	//	{
	//		return AG_FAILURE;
	//	}
	//	pinRaster = pinFolder->GetRasterDataset()->GetRaster(inRasterName);
	//	
	//	RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();

	//	poutRaster = pRasterFactory->CreateRasterPNG(outRasterName, pinRaster->GetWidth(), pinRaster->GetHeight());
	//	//poutRaster = pRasterFactory->CreateRaster(outRasterName, pinRaster->GetExtent(), pinRaster);
	//	if(poutRaster==NULL)
	//	{
	//		pinFolder->Release();
	//		return AG_FAILURE;
	//	}

	//	if(!Stretch(pinRaster, poutRaster))
	//	{
	//		pinFolder->Release();
	//		poutRaster->Release();
	//		return AG_FAILURE;
	//	}

	//	poutFolder = poutRasterWorkspace->GetFolder(outRasterPath);
	//	if(poutFolder==NULL)
	//	{
	//		pinFolder->Release();
	//		poutRaster->Release();
	//		return AG_FAILURE;
	//	}
	//	RESULTCODE rc = poutFolder->GetRasterDataset()->AddRaster(outRasterName, poutRaster);
	//	poutRaster->Release();
	//	pinFolder->Release();
	//	poutFolder->Release();

	//	return rc;
	//}

	RESULTCODE RasterStretchProcessorImpl::Execute()
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

		//poutRaster = pRasterFactory->CreateRasterPNG(outRasterName, pinRaster->GetWidth(), pinRaster->GetHeight());
		//poutRaster = pRasterFactory->CreateRaster(outRasterName, pinRaster->GetExtent(), pinRaster);

		poutRaster = Stretch(pinRaster);
		if(poutRaster==NULL)
		{
			pinFolder->Release();
			return AG_FAILURE;
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

	Raster*	RasterStretchProcessorImpl::Stretch_Byte(Raster* pinRaster)
	{
		Raster* poutRaster = NULL;
		g_uint nbands = pinRaster->GetBandCount();
		switch(nbands)
		{
		case 1:
			poutRaster = Stretch_Byte_1(pinRaster);
			break;
		//case 3:
		//	break;
		default:
			poutRaster = Stretch_Byte_n(pinRaster);
		}
		return poutRaster;
	}

	Raster*	RasterStretchProcessorImpl::Stretch_Byte_1(Raster* pinRaster)
	{
		return NULL;
	}

	Raster* RasterStretchProcessorImpl::Stretch_Byte_n(Raster* pinRaster)
	{
		Raster* poutRaster = NULL;
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();
		const char* raster_name = GetOutputRaster();

		poutRaster = pRasterFactory->CreateRaster(raster_name, pinRaster->GetExtent(), pinRaster);
		if(poutRaster==NULL)
		{
			return NULL;
		}

		RasterBand* pinBand = NULL;
		RasterBand* poutBand = NULL;
		g_uint nband = poutRaster->GetBandCount();
		size_t pixel_count = poutRaster->GetWidth() * poutRaster->GetHeight();
		g_byte* i_data = NULL;
		//g_byte* ptr = NULL;
		g_byte v_min=0, v_max=0, value, value_span;
		float  scale = 1.0f;

		for(g_uint i=0; i<nband; i++)
		//for(g_uint i=1; i<nband; i++)
		{
			pinBand = pinRaster->GetBand(i);
			poutBand= poutRaster->GetBand(i);
			i_data = (g_byte*)pinBand->GetData();

			pinBand->GetMinMaxValue(v_min, v_max);
			value_span = v_max - v_min;

			g_byte* ptr=i_data;
			for(g_uint j=0; j<pixel_count; j++, ptr++)
			{
				value = *ptr;
				scale = ((float)value - (float)v_min) / (float)value_span;
				*ptr = scale * AUGE_BYTE_MAX;
			}
			poutBand->SetData(i_data);
		}

		return poutRaster;
	}

	Raster*	RasterStretchProcessorImpl::Stretch_Short(Raster* pinRaster)
	{
		Raster* poutRaster = NULL;
		g_uint nbands = pinRaster->GetBandCount();
		switch(nbands)
		{
		case 1:
			poutRaster = Stretch_Short_1(pinRaster);
			break;
			//case 3:
			//	break;
		default:
			poutRaster = Stretch_Short_n(pinRaster);
		}
		return poutRaster;
	}

	Raster*	RasterStretchProcessorImpl::Stretch_Short_1(Raster* pinRaster)
	{
		Raster* poutRaster = NULL;
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();
		const char* raster_name = GetOutputRaster();

		poutRaster = pRasterFactory->CreateRaster(raster_name, pinRaster->GetExtent(), pinRaster);
		if(poutRaster==NULL)
		{
			return NULL;
		}

		RasterBand* pinBand = NULL;
		RasterBand* poutBand = NULL;
		g_uint nband = poutRaster->GetBandCount();
		size_t pixel_count = poutRaster->GetWidth() * poutRaster->GetHeight();
		short* i_data = NULL;
		//short* ptr = NULL;
		short v_min=0, v_max=0, value, value_span;
		float  scale = 1.0f;

		for(g_uint i=0; i<nband; i++)
			//for(g_uint i=1; i<nband; i++)
		{
			pinBand = pinRaster->GetBand(i);
			poutBand= poutRaster->GetBand(i);
			i_data = (short*)pinBand->GetData();

			pinBand->GetMinMaxValue(v_min, v_max);
			value_span = v_max - v_min;

			short* ptr=i_data;
			for(g_uint j=0; j<pixel_count; j++, ptr++)
			{
				value = *ptr;
				scale = ((float)value - (float)v_min) / (float)value_span;
				*ptr = scale * AUGE_SHORT_MAX;
			}
			poutBand->SetData(i_data);
		}

		return poutRaster;
	}

	Raster* RasterStretchProcessorImpl::Stretch_Short_n(Raster* pinRaster)
	{
		Raster* poutRaster = NULL;
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();
		const char* raster_name = GetOutputRaster();

		poutRaster = pRasterFactory->CreateRaster(raster_name, pinRaster->GetExtent(), pinRaster);
		if(poutRaster==NULL)
		{
			return NULL;
		}

		RasterBand* pinBand = NULL;
		RasterBand* poutBand = NULL;
		g_uint nband = poutRaster->GetBandCount();
		size_t pixel_count = poutRaster->GetWidth() * poutRaster->GetHeight();
		short* i_data = NULL;
		//short* ptr = NULL;
		short v_min=0, v_max=0, value, value_span;
		float  scale = 1.0f;

		for(g_uint i=0; i<nband; i++)
			//for(g_uint i=1; i<nband; i++)
		{
			pinBand = pinRaster->GetBand(i);
			poutBand= poutRaster->GetBand(i);
			i_data = (short*)pinBand->GetData();

			pinBand->GetMinMaxValue(v_min, v_max);
			value_span = v_max - v_min;

			short* ptr=i_data;
			for(g_uint j=0; j<pixel_count; j++, ptr++)
			{
				value = *ptr;
				scale = ((float)value - (float)v_min) / (float)value_span;
				*ptr = scale * AUGE_SHORT_MAX;
			}
			poutBand->SetData(i_data);
		}

		return poutRaster;
	}


	Raster*	RasterStretchProcessorImpl::Stretch_Float(Raster* pinRaster)
	{
		Raster* poutRaster = NULL;
		g_uint nbands = pinRaster->GetBandCount();
		switch(nbands)
		{
		case 1:
			poutRaster = Stretch_Float_1(pinRaster);
			break;
			//case 3:
			//	break;
		default:
			poutRaster = Stretch_Float_n(pinRaster);
		}
		return poutRaster;
	}

	Raster*	RasterStretchProcessorImpl::Stretch_Float_1(Raster* pinRaster)
	{
		Raster* poutRaster = NULL;
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();
		const char* raster_name = GetOutputRaster();

		poutRaster = pRasterFactory->CreateRaster("", pinRaster->GetExtent(), pinRaster);
		if(poutRaster==NULL)
		{
			return NULL;
		}

		RasterBand* pinBand = NULL;
		RasterBand* poutBand = NULL;
		g_uint nband = poutRaster->GetBandCount();
		size_t pixel_count = poutRaster->GetWidth() * poutRaster->GetHeight();
		float* i_data = NULL;
		//float* ptr = NULL;
		float v_min=0, v_max=0, value, value_span;
		float  scale = 1.0f;

		for(g_uint i=0; i<nband; i++)
			//for(g_uint i=1; i<nband; i++)
		{
			pinBand = pinRaster->GetBand(i);
			poutBand= poutRaster->GetBand(i);
			i_data = (float*)pinBand->GetData();

			pinBand->GetMinMaxValue(v_min, v_max);
			value_span = v_max - v_min;

			float* ptr=i_data;
			for(g_uint j=0; j<pixel_count; j++, ptr++)
			{
				value = *ptr;
				scale = ((float)value - (float)v_min) / (float)value_span;
				*ptr = scale * AUGE_FLOAT_MAX;
			}
			poutBand->SetData(i_data);
		}

		return poutRaster;
	}

	Raster* RasterStretchProcessorImpl::Stretch_Float_n(Raster* pinRaster)
	{
		Raster* poutRaster = NULL;
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();
		const char* raster_name = GetOutputRaster();

		poutRaster = pRasterFactory->CreateRaster(raster_name, pinRaster->GetExtent(), pinRaster);
		if(poutRaster==NULL)
		{
			return NULL;
		}

		RasterBand* pinBand = NULL;
		RasterBand* poutBand = NULL;
		g_uint nband = poutRaster->GetBandCount();
		size_t pixel_count = poutRaster->GetWidth() * poutRaster->GetHeight();
		float* i_data = NULL;
		//float* ptr = NULL;
		float v_min=0, v_max=0, value, value_span;
		float  scale = 1.0f;

		for(g_uint i=0; i<nband; i++)
			//for(g_uint i=1; i<nband; i++)
		{
			pinBand = pinRaster->GetBand(i);
			poutBand= poutRaster->GetBand(i);
			i_data = (float*)pinBand->GetData();

			pinBand->GetMinMaxValue(v_min, v_max);
			value_span = v_max - v_min;

			float* ptr=i_data;
			for(g_uint j=0; j<pixel_count; j++, ptr++)
			{
				value = *ptr;
				scale = ((float)value - (float)v_min) / (float)value_span;
				*ptr = scale * AUGE_FLOAT_MAX;
			}
			poutBand->SetData(i_data);
		}

		return poutRaster;
	}

	void RasterStretchProcessorImpl::Release()
	{
		delete this;
	}

	///
	Raster* RasterStretchProcessorImpl::Stretch(Raster* pinRaster)
	{
		Raster* poutRaster = NULL;
		augePixelType pixelType = pinRaster->GetPixelType();
		switch(pixelType)
		{
		case augePixelByte:
			poutRaster = Stretch_Byte(pinRaster);
			break;
		case augePixelUInt16:
		case augePixelInt16:
			poutRaster = Stretch_Short(pinRaster);
			break;
		case augePixelUInt32:
		case augePixelInt32:
			break;
		case augePixelFloat32:
			poutRaster = Stretch_Float(pinRaster);
			break;
		case augePixelDouble:
			break;
		}
		return poutRaster;
	}
	/*
	bool RasterStretchProcessorImpl::Stretch(Raster* pinRaster, Raster* poutRaster)
	{
	bool ret = true;
	GLogger* pLogger = augeGetLoggerInstance();

	augePixelType pixelType = pinRaster->GetPixelType();
	switch(pixelType)
	{
	case augePixelByte:
	ret = Stretch_Byte(poutRaster, pinRaster);
	break;
	case augePixelUInt16:
	case augePixelInt16:
	ret = Stretch_Short(poutRaster, pinRaster);
	break;
	case augePixelUInt32:
	case augePixelInt32:
	break;
	case augePixelFloat32:
	break;
	case augePixelDouble:
	break;
	}
	return ret;
	}*/

	//////////////////////////////////////////////////////////////////////////
	// General Stretch
	//bool RasterStretchProcessorImpl::Stretch_Short(Raster* pinRaster, Raster* poutRaster)
	//{
	//	g_uint nband = 0;
	//	RasterBand* pinBand = NULL;
	//	RasterBand* poutBand = NULL;

	//	nband = pinRaster->GetBandCount();
	//	for(g_uint i=0; i<nband; i++)
	//	{
	//		pinBand = pinRaster->GetBand(i);
	//		poutBand = poutRaster->GetBand(i);

	//		Stretch_Band_Short(pinBand, poutBand);
	//	}

	//	return true;
	//}

	//bool RasterStretchProcessorImpl::Stretch_Band_Short(RasterBand* pinBand, RasterBand* poutBand)
	//{
	//	short v_min=0, v_max=0;
	//	pinBand->GetMinMaxValue(v_min, v_max);
	//	
	//	short* data = (short*)pinBand->GetData();
	//	short value_span = v_max - v_min;

	//	size_t pixel_count = poutBand->GetWidth() * poutBand->GetHeight();
	//	size_t buffer_size = pixel_count * sizeof(g_int16);
	//	return true;
	//}
	// General Stretch End
	//////////////////////////////////////////////////////////////////////////

	bool RasterStretchProcessorImpl::Stretch_Byte(Raster* pinRaster, Raster* poutRaster)
	{
		bool ret = false;
		g_uint nband = pinRaster->GetBandCount();
		switch(nband)
		{
		case 1:
			ret = Stretch_Byte_1(pinRaster, poutRaster);
			break;
		default:
			ret = Stretch_Byte_n(pinRaster, poutRaster);
			break;
		}
		return ret;
	}

	bool RasterStretchProcessorImpl::Stretch_Byte_1(Raster* pinRaster, Raster* poutRaster)
	{
		GLogger* pLogger = augeGetLoggerInstance();

		RasterBand* pinBand = pinRaster->GetBand(0);

		RasterBand* poutBand_R = poutRaster->GetBand(0);
		RasterBand* poutBand_G = poutRaster->GetBand(1);
		RasterBand* poutBand_B = poutRaster->GetBand(2);
		RasterBand* poutBand_A = poutRaster->GetBand(3);

		size_t size = sizeof(short);

		augePixelType pixelType = pinRaster->GetPixelType();
		size_t pixel_count = poutRaster->GetWidth() * poutRaster->GetHeight();
		size_t buffer_size = pixel_count * sizeof(g_byte);
		g_byte* r_b = (g_byte*)malloc(buffer_size);
		g_byte* g_b = (g_byte*)malloc(buffer_size);
		g_byte* b_b = (g_byte*)malloc(buffer_size);
		memset(r_b, 0, buffer_size);
		memset(g_b, 0, buffer_size);
		memset(b_b, 0, buffer_size);

		g_byte v_min=0, v_max=0;
		pinBand->GetMinMaxValue(v_min, v_max);
		g_byte* data = (g_byte*)pinBand->GetData();

		g_byte value_span = v_max - v_min;
		g_byte r_s = m_color_start.GetRed();
		g_byte g_s = m_color_start.GetGreen();
		g_byte b_s = m_color_start.GetBlue();
		g_byte r_e = m_color_end.GetRed();
		g_byte g_e = m_color_end.GetGreen();
		g_byte b_e = m_color_end.GetBlue();

		float h_s=0.0f, s_s=0.0f, v_s=0.0f;
		float h_e=0.0f, s_e=0.0f, v_e=0.0f;

		auge_rgb_2_hsv(r_s, g_s, b_s, h_s, s_s, v_s);
		auge_rgb_2_hsv(r_e, g_e, b_e, h_e, s_e, v_e);

		float h_span = h_e - h_s;
		float s_span = s_e - s_s;
		float v_span = v_e - v_s;
		float scale = 1.0f;

		float h=0.0f, s=0.0f, v=0.0f;
		g_byte r=0, g=0, b=0;

		data[0] = 1000;
		data[1] = 3000;
		g_byte value = 0;
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

	bool RasterStretchProcessorImpl::Stretch_Byte_n(Raster* pinRaster, Raster* poutRaster)
	{
		g_uint nband = pinRaster->GetBandCount();
		RasterBand* pinBand = NULL;
		RasterBand* poutBand = NULL;
		size_t pixel_count = poutRaster->GetWidth() * poutRaster->GetHeight();
		g_byte* i_data = NULL;
		//g_byte* ptr = NULL;
		g_byte v_min=0, v_max=0, value, value_span;
		float  scale = 1.0f;
		
		for(g_uint i=0; i<nband; i++)
		//for(g_uint i=1; i<nband; i++)
		{
			pinBand = pinRaster->GetBand(i);
			poutBand= poutRaster->GetBand(i);
			i_data = (g_byte*)pinBand->GetData();
			
			pinBand->GetMinMaxValue(v_min, v_max);
			value_span = v_max - v_min;

			g_byte* ptr=i_data;
			for(g_uint i=0; i<pixel_count; i++, ptr++)
			{
				value = *ptr;
				scale = ((float)value - (float)v_min) / (float)value_span;
				*ptr = scale * AUGE_BYTE_MAX;
			}
			poutBand->SetData(i_data);
		}

		return true;
	}

	bool RasterStretchProcessorImpl::Stretch_Band_Byte(RasterBand* pinBand, RasterBand* poutBand)
	{
		/*size_t pixel_count = poutRaster->GetWidth() * poutRaster->GetHeight();
		size_t buffer_size = pixel_count * sizeof(g_byte);
		g_byte* r_b = (g_byte*)malloc(buffer_size);
		g_byte* g_b = (g_byte*)malloc(buffer_size);
		g_byte* b_b = (g_byte*)malloc(buffer_size);
		memset(r_b, 0, buffer_size);
		memset(g_b, 0, buffer_size);
		memset(b_b, 0, buffer_size);

		g_byte v_min=0, v_max=0;
		pinBand->GetMinMaxValue(v_min, v_max);
		g_byte* data = (g_byte*)pinBand->GetData();

		g_byte value_span = v_max - v_min;
		g_byte r_s = m_color_start.GetRed();
		g_byte g_s = m_color_start.GetGreen();
		g_byte b_s = m_color_start.GetBlue();
		g_byte r_e = m_color_end.GetRed();
		g_byte g_e = m_color_end.GetGreen();
		g_byte b_e = m_color_end.GetBlue();

		float h_s=0.0f, s_s=0.0f, v_s=0.0f;
		float h_e=0.0f, s_e=0.0f, v_e=0.0f;

		auge_rgb_2_hsv(r_s, g_s, b_s, h_s, s_s, v_s);
		auge_rgb_2_hsv(r_e, g_e, b_e, h_e, s_e, v_e);

		float h_span = h_e - h_s;
		float s_span = s_e - s_s;
		float v_span = v_e - v_s;
		float scale = 1.0f;

		float h=0.0f, s=0.0f, v=0.0f;
		g_byte r=0, g=0, b=0;

		data[0] = 1000;
		data[1] = 3000;
		g_byte value = 0;
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
		}

		poutBand_R->SetData(r_b);
		poutBand_G->SetData(g_b);
		poutBand_B->SetData(b_b);

		memset(r_b, 255, buffer_size);
		poutBand_A->SetData(r_b);

		free(r_b);
		free(g_b);
		free(b_b);
*/

		return true;
	}

	bool RasterStretchProcessorImpl::Stretch_Short(Raster* pinRaster, Raster* poutRaster)
	{
		GLogger* pLogger = augeGetLoggerInstance();

		RasterBand* pinBand = pinRaster->GetBand(0);

		RasterBand* poutBand_R = poutRaster->GetBand(0);
		RasterBand* poutBand_G = poutRaster->GetBand(1);
		RasterBand* poutBand_B = poutRaster->GetBand(2);
		RasterBand* poutBand_A = poutRaster->GetBand(3);

		size_t size = sizeof(short);

		augePixelType pixelType = pinRaster->GetPixelType();
		size_t pixel_count = poutRaster->GetWidth() * poutRaster->GetHeight();
		size_t buffer_size = pixel_count * sizeof(g_byte);
		g_byte* r_b = (g_byte*)malloc(buffer_size);
		g_byte* g_b = (g_byte*)malloc(buffer_size);
		g_byte* b_b = (g_byte*)malloc(buffer_size);
		memset(r_b, 0, buffer_size);
		memset(g_b, 0, buffer_size);
		memset(b_b, 0, buffer_size);

		short v_min=0, v_max=0;
		pinBand->GetMinMaxValue(v_min, v_max);
		short* data = (short*)pinBand->GetData();

		short value_span = v_max - v_min;
		g_byte r_s = m_color_start.GetRed();
		g_byte g_s = m_color_start.GetGreen();
		g_byte b_s = m_color_start.GetBlue();
		g_byte r_e = m_color_end.GetRed();
		g_byte g_e = m_color_end.GetGreen();
		g_byte b_e = m_color_end.GetBlue();

		char color[AUGE_NAME_MAX];
		g_sprintf(color, "[Start Color]:r=%c,	g=%c,	b=%c\n", r_s, g_s, b_s);
		pLogger->Info(color, __FILE__, __LINE__);
		g_sprintf(color, "[End Color]:r=%c,	g=%c,	b=%c\n", r_e, g_e, b_e);
		pLogger->Info(color, __FILE__, __LINE__);

		float h_s=0.0f, s_s=0.0f, v_s=0.0f;
		float h_e=0.0f, s_e=0.0f, v_e=0.0f;

		auge_rgb_2_hsv(r_s, g_s, b_s, h_s, s_s, v_s);
		auge_rgb_2_hsv(r_e, g_e, b_e, h_e, s_e, v_e);

		float h_span = h_e - h_s;
		float s_span = s_e - s_s;
		float v_span = v_e - v_s;
		float scale = 1.0f;

		float h=0.0f, s=0.0f, v=0.0f;
		g_byte r=0, g=0, b=0;

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

	//bool RasterStretchProcessorImpl::Stretch(Raster* pinRaster, Raster* poutRaster)
	//{
	//	GLogger* pLogger = augeGetLoggerInstance();

	//	RasterBand* pinBand = pinRaster->GetBand(0);

	//	RasterBand* poutBand_R = poutRaster->GetBand(0);
	//	RasterBand* poutBand_G = poutRaster->GetBand(1);
	//	RasterBand* poutBand_B = poutRaster->GetBand(2);
	//	RasterBand* poutBand_A = poutRaster->GetBand(3);

	//	size_t size = sizeof(short);

	//	augePixelType pixelType = pinRaster->GetPixelType();
	//	size_t pixel_count = poutRaster->GetWidth() * poutRaster->GetHeight();
	//	size_t buffer_size = pixel_count * sizeof(g_byte);
	//	g_byte* r_b = (g_byte*)malloc(buffer_size);
	//	g_byte* g_b = (g_byte*)malloc(buffer_size);
	//	g_byte* b_b = (g_byte*)malloc(buffer_size);
	//	memset(r_b, 0, buffer_size);
	//	memset(g_b, 0, buffer_size);
	//	memset(b_b, 0, buffer_size);

	//	short v_min=0, v_max=0;
	//	pinBand->GetMinMaxValue(v_min, v_max);
	//	short* data = (short*)pinBand->GetData();

	//	short value_span = v_max - v_min;
	//	g_byte r_s = m_color_start.GetRed();
	//	g_byte g_s = m_color_start.GetGreen();
	//	g_byte b_s = m_color_start.GetBlue();
	//	g_byte r_e = m_color_end.GetRed();
	//	g_byte g_e = m_color_end.GetGreen();
	//	g_byte b_e = m_color_end.GetBlue();

	//	char color[AUGE_NAME_MAX];
	//	g_sprintf(color, "[Start Color]:r=%c,	g=%c,	b=%c\n", r_s, g_s, b_s);
	//	pLogger->Info(color, __FILE__, __LINE__);
	//	g_sprintf(color, "[End Color]:r=%c,	g=%c,	b=%c\n", r_e, g_e, b_e);
	//	pLogger->Info(color, __FILE__, __LINE__);

	//	float h_s=0.0f, s_s=0.0f, v_s=0.0f;
	//	float h_e=0.0f, s_e=0.0f, v_e=0.0f;
	//	
	//	auge_rgb_2_hsv(r_s, g_s, b_s, h_s, s_s, v_s);
	//	auge_rgb_2_hsv(r_e, g_e, b_e, h_e, s_e, v_e);
	//	
	//	float h_span = h_e - h_s;
	//	float s_span = s_e - s_s;
	//	float v_span = v_e - v_s;
	//	float scale = 1.0f;

	//	float h=0.0f, s=0.0f, v=0.0f;
	//	g_byte r=0, g=0, b=0;

	//	data[0] = 1000;
	//	data[1] = 3000;
	//	short value = 0;
	//	for(int i=0; i<pixel_count; i++)
	//	{
	//		value = data[i];
	//		scale = ((float)value - (float)v_min) / (float)value_span;

	//		h = h_span * scale + h_s;
	//		s = s_span * scale + s_s;
	//		v = v_span * scale + v_s;

	//		auge_hsv_2_rgb(h, s, v, r, g, b);
	//		r_b[i] = r;
	//		g_b[i] = g;
	//		b_b[i] = b;

	//		//printf("[%d]:\t[%f\t%f\t%f]\t[%d\t%d\t%d]\n",value, h, s, v, r,g,b);
	//	}

	//	poutBand_R->SetData(r_b);
	//	poutBand_G->SetData(g_b);
	//	poutBand_B->SetData(b_b);

	//	memset(r_b, 255, buffer_size);
	//	poutBand_A->SetData(r_b);

	//	free(r_b);
	//	free(g_b);
	//	free(b_b);

	//	return true;
	//}

	void RasterStretchProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}
}

