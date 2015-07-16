#include "RasterEdgeDetectProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"

#include <math.h>

#include <fstream>

namespace auge
{
	int sobel_x[3][3] = {-1,-2,-1,
						  0, 0, 0,
						  1, 2, 1};
	int sobel_y[3][3] = {-1, 0, 1,
		  				 -2, 0, 2,
						 -1, 0, 1};

	RasterEdgeDetectProcessorImpl::RasterEdgeDetectProcessorImpl()
	{
	}

	RasterEdgeDetectProcessorImpl::~RasterEdgeDetectProcessorImpl()
	{

	}
	
	void RasterEdgeDetectProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void RasterEdgeDetectProcessorImpl::SetInputRaster(const char* rasterName)
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

	void RasterEdgeDetectProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void RasterEdgeDetectProcessorImpl::SetOutputRaster(const char* rasterName)
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
	
	void RasterEdgeDetectProcessorImpl::SetEdgeDetector(augeEdgeDetector detector)
	{
		m_detector = detector;
	}

	const char*	RasterEdgeDetectProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	RasterEdgeDetectProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	RasterEdgeDetectProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterEdgeDetectProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	RESULTCODE RasterEdgeDetectProcessorImpl::Execute()
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

		switch(m_detector)
		{
		case augeEdgeSobel:
			poutRaster = Sobel(pinRaster);
			break;
		case augeEdgeRoberts:
			break;
		case augeEdgePrewitt:
			break;
		case augeEdgeLaplacian:
			break;
		}

		//if(!Stretch(pinRaster, poutRaster))
		//{
		//	pinRasterDataset->Release();
		//	poutRaster->Release();
		//	return AG_FAILURE;
		//}

		if(poutRaster!=NULL)
		{
			poutRaster->SetName(outRasterName);
		}
		RESULTCODE rc = poutRasterWorkspace->AddRaster(poutRaster);
		poutRaster->Release();
		pinRasterDataset->Release();

		return rc;

		return AG_SUCCESS;
	}

	void RasterEdgeDetectProcessorImpl::Release()
	{
		delete this;
	}

	Raster* RasterEdgeDetectProcessorImpl::Sobel(Raster* pinRaster)
	{
		Raster* poutRaster = NULL;
		int band_count = pinRaster->GetBandCount();
		if(band_count==1)
		{
			poutRaster = Sobel_1(pinRaster);
		}
		else if(band_count>=3)
		{
			poutRaster = Sobel_3(pinRaster);
		}
		else
		{
			const char* msg = "Raster band count should be 1 or >3";
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
		}

		return poutRaster;
	}

	Raster* RasterEdgeDetectProcessorImpl::Sobel_1(Raster* pinRaster)
	{
		RasterBand* pinBand = pinRaster->GetBand(0);
		if(pinBand==NULL)
		{
			return NULL;
		}

		Raster* poutRaster = NULL;
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();
		poutRaster = pRasterFactory->CreateRaster("", pinRaster->GetExtent(), pinRaster);
		if(poutRaster==NULL)
		{
			return NULL;
		}
		
		RasterBand* poutBand = poutRaster->GetBand(0);

		RESULTCODE rc = Sobel(pinBand, poutBand);
		if(rc!=AG_SUCCESS)
		{
			poutRaster->Release();
			return NULL;
		}
		
		return poutRaster;
	}

	Raster* RasterEdgeDetectProcessorImpl::Sobel_3(Raster* pinRaster)
	{
		Raster* poutRaster = NULL;
		augePixelType type = pinRaster->GetPixelType();
		switch(type)
		{
		case augePixelByte:
			poutRaster = Sobel_Byte(pinRaster);
			break;
		case augePixelUInt16:
		case augePixelInt16:
			//poutRaster = Sobel_Short(pinRaster);
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

	RESULTCODE RasterEdgeDetectProcessorImpl::Sobel(RasterBand* pBand)
	{
		augePixelType type = pBand->GetPixelType();
		switch(type)
		{
		case augePixelByte:			
			break;
		case augePixelUInt16:
		case augePixelInt16:
			//Sobel_Short(pBand);
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

	RESULTCODE RasterEdgeDetectProcessorImpl::Sobel(RasterBand* pinBand, RasterBand* poutBand)
	{
		augePixelType type = pinBand->GetPixelType();
		switch(type)
		{
		case augePixelByte:			
			break;
		case augePixelUInt16:
		case augePixelInt16:
			Sobel_Short(pinBand, poutBand);
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

	RESULTCODE RasterEdgeDetectProcessorImpl::Sobel_Short(RasterBand* pinBand, RasterBand* poutBand)
	{
		g_uint width = pinBand->GetWidth();
		g_uint height= pinBand->GetHeight();
		
		short v_h=0, v_v=0;
		short* data = (short*)pinBand->GetData();
		short* ptr_0 = data + 1; 
		short* ptr_1 = ptr_0 + width;
		short* ptr_2 = ptr_1 + width;
		short  v_min,v_max;

		//pinBand->GetMinMaxValue(v_min,v_max);
		//Normalize(data, width, height, v_min,v_max);

		g_int64 size = width*height*sizeof(short);
		short* edge = (short*)malloc(size);
		memset(edge, 0, size);

		short* ptr = edge + width + 1;
		for(g_uint i=1; i<height-1; i++)
		{	
			for(g_uint j=1; j<width-1; j++,ptr_0++,ptr_1++,ptr_2++,ptr++)
			{
				v_h = sobel_x[0][0]*ptr_0[-1] + sobel_x[0][1]*ptr_0[0] + sobel_x[0][2]*ptr_0[1]
					+ sobel_x[1][0]*ptr_1[-1] + sobel_x[1][1]*ptr_1[0] + sobel_x[1][2]*ptr_1[1]
					+ sobel_x[2][0]*ptr_2[-1] + sobel_x[2][1]*ptr_2[0] + sobel_x[2][2]*ptr_2[1];

				v_v = sobel_y[0][0]*ptr_0[-1] + sobel_y[0][1]*ptr_0[0] + sobel_y[0][2]*ptr_0[1]
					+ sobel_y[1][0]*ptr_1[-1] + sobel_y[1][1]*ptr_1[0] + sobel_y[1][2]*ptr_1[1]
					+ sobel_y[2][0]*ptr_2[-1] + sobel_y[2][1]*ptr_2[0] + sobel_y[2][2]*ptr_2[1];

				*ptr = (short)sqrt(pow(v_h,2.0f) + pow(v_v,2.0f));
			}
			ptr_0 += 2;
			ptr_1 += 2;
			ptr_2 += 2;
			ptr  += 2;
		}

		ptr   = edge;
		poutBand->SetData(edge);
		free(edge);

		return AG_SUCCESS;
	}

	Raster* RasterEdgeDetectProcessorImpl::Sobel_Byte(Raster* pinRaster)
	{
		g_uint64 w = pinRaster->GetWidth();
		g_uint64 h = pinRaster->GetHeight();
		g_uint64 size = w * h;

		RasterBand* prBand = pinRaster->GetBand(0);
		RasterBand* pgBand = pinRaster->GetBand(1);
		RasterBand* pbBand = pinRaster->GetBand(2);

		g_uchar* r = (g_uchar*)(prBand->GetData());
		g_uchar* g = (g_uchar*)(pgBand->GetData());
		g_uchar* b = (g_uchar*)(pbBand->GetData());

		//Graylize
		//Gray = R*0.299 + G*0.587 + B*0.114
		for(g_uint64 i=0; i<size; i++)
		{
			r[i] = (r[i]*299 + g[i]*587 + b[i]*114) / 1000;
		}

		g_uchar  v_h=0, v_v=0, v;
		g_uchar* ptr_0 = r + 1; 
		g_uchar* ptr_1 = ptr_0 + w;
		g_uchar* ptr_2 = ptr_1 + w;
		g_uchar* ptr   = g + w + 1;
		g_uint i=0, j=0;
		
		ptr   = g + w + 1;
		memset(g, 0, size);
		for(i=1; i<h-1; i++)
		{	
			for(j=1; j<w-1; j++,ptr_0++,ptr_1++,ptr_2++, ptr++)
			{
				v_h = sobel_x[0][0]*ptr_0[-1] + sobel_x[0][1]*ptr_0[0] + sobel_x[0][2]*ptr_0[1]
					+ sobel_x[1][0]*ptr_1[-1] + sobel_x[1][1]*ptr_1[0] + sobel_x[1][2]*ptr_1[1]
					+ sobel_x[2][0]*ptr_2[-1] + sobel_x[2][1]*ptr_2[0] + sobel_x[2][2]*ptr_2[1];

				v_v = sobel_x[0][0]*ptr_0[-1] + sobel_x[0][1]*ptr_0[0] + sobel_x[0][2]*ptr_0[1]
					+ sobel_x[1][0]*ptr_1[-1] + sobel_x[1][1]*ptr_1[0] + sobel_x[1][2]*ptr_1[1]
					+ sobel_x[2][0]*ptr_2[-1] + sobel_x[2][1]*ptr_2[0] + sobel_x[2][2]*ptr_2[1];

				v = (g_uchar)sqrt(pow(v_h,2.0f) + pow(v_v,2.0f));
				ptr[0] = v < 100 ? 0 : 255;
			}
			ptr_0 += 2;
			ptr_1 += 2;
			ptr_2 += 2;
			ptr   += 2;
		}

		// write to image
		prBand->SetData(g);
		pgBand->SetData(g);
		pbBand->SetData(g);

		free(r);
		free(g);
		free(b);

		pinRaster->AddRef();
		return pinRaster;
	}

	void RasterEdgeDetectProcessorImpl::Normalize(short* data, int width, int height, short v_min, short v_max)
	{
		short* ptr = data;
		short  span = v_max - v_min;
		g_int64 count = ((g_int64)width) * ((g_int64)width);
		for(g_int64 i=0; i<count; i++, ptr++)
		{
			double scale = (double)((*ptr) - v_min) / (double)span;
			*ptr = scale * 255;
		}
	}
}
