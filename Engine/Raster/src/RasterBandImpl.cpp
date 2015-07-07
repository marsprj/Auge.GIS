#include "RasterImpl.h"
#include "RasterBandImpl.h"

#include "AugeCore.h"

namespace auge
{	
	RasterBandImpl::RasterBandImpl()
	{
		m_poRasterBand = NULL;
		m_data = NULL;
		m_pixel_size = 1;
		m_data_size = 0;
		m_pRaster = NULL;
	}

	RasterBandImpl::~RasterBandImpl()
	{
		if(m_data)
		{
			free(m_data);
			m_data = NULL;
		}
	}

	g_uint RasterBandImpl::GetWidth()
	{
		return m_poRasterBand->GetXSize();
	}

	g_uint RasterBandImpl::GetHeight()
	{
		return m_poRasterBand->GetYSize();
	}

	augePixelType RasterBandImpl::GetPixelType()
	{
		return (augePixelType)m_poRasterBand->GetRasterDataType();
	}

	g_uint RasterBandImpl::GetPixelSize()
	{
		return m_pixel_size;
	}

	void* RasterBandImpl::GetData()
	{
		if(m_data==NULL)
		{
			m_data = malloc(m_data_size);
			memset(m_data, 0, m_data_size);

			int width = m_poRasterBand->GetXSize();
			int height = m_poRasterBand->GetYSize();

			//CPLErr err = m_poRasterBand->RasterIO(GF_Read,
			//										0,0,
			//										width,height,
			//										m_data,
			//										width,height,
			//										(GDALDataType)(m_pRaster->GetPixelType()),
			//										1,width);
			CPLErr err = m_poRasterBand->RasterIO(GF_Read,
				0,0,
				width,height,
				m_data,
				width,height,
				(GDALDataType)(m_pRaster->GetPixelType()),
				0,0);
			if(err!=CE_None)
			{
				const char* msg = CPLGetLastErrorMsg();
				GError* pError = augeGetErrorInstance();
				pError->SetError(msg);

				free(m_data);
				m_data = NULL;
			}
		}
		return m_data;
	}

	GEnvelope& RasterBandImpl::GetExtent()
	{
		return m_pRaster->GetExtent();
	}

	g_uint RasterBandImpl::GetDataSize()
	{
		return m_data_size;
	}

	void* RasterBandImpl::GetData(int x, int y)
	{
		void* data = GetData();
		if(data==NULL)
		{
			return NULL;
		}
		g_uint offset = (x + y * GetWidth()) * m_pixel_size;
		return (unsigned char*)data + offset;
	}

	void* RasterBandImpl::GetData(double x, double y)
	{
		return NULL;
	}

	RESULTCODE RasterBandImpl::GetData(void* data, GRect rect)
	{
		if(data==NULL)
		{
			return AG_FAILURE;
		}
		int x = rect.m_xmin;
		int y = rect.m_ymin;
		int width = rect.GetWidth();
		int height= rect.GetHeight();
		GDALDataType type = m_poRasterBand->GetRasterDataType();
		CPLErr err = m_poRasterBand->RasterIO(	GF_Read, 
			x, y,
			width, height, 
			data,
			width, height,
			type, 
			0, 0);
		if(err!=CE_None)
		{
			return AG_FAILURE;
		}

		return AG_SUCCESS;
	}

	bool RasterBandImpl::SetData(void* data)
	{
		if(data==NULL)
		{
			return false;
		}

		int width = GetWidth();
		int height= GetHeight();
		GDALDataType type = m_poRasterBand->GetRasterDataType();
		CPLErr err = m_poRasterBand->RasterIO(	GF_Write, 
			0, 0,
			width, height, 
			data,
			width, height,
			type, 
			0, 0);
		if(err!=CE_None)
		{
			return false;
		}

		return true;
	}


	bool RasterBandImpl::GetMinMaxValue(short& minv, short& maxv)
	{
		short *idata = (short*)GetData();

		int count = m_pRaster->GetWidth() * m_pRaster->GetHeight();
		short v_min = AUGE_INT_MAX;
		short v_max = AUGE_INT_MIN;
		for(int i=0; i<count; i++)
		{
			short value = idata[i];			
			v_min = v_min < value ? v_min : value;
			v_max = v_max > value ? v_max : value;
		}
		minv = v_min;
		maxv = v_max;
		return true;
	}

	bool RasterBandImpl::GetMinMaxValue(int& minv, int& maxv)
	{
		int *idata = (int*)GetData();

		int count = m_pRaster->GetWidth() * m_pRaster->GetHeight();
		int v_min = AUGE_INT_MAX;
		int v_max = AUGE_INT_MIN;
		for(int i=0; i<count; i++)
		{
			v_min = v_min < idata[i] ? v_min : idata[i];
			v_max = v_max > idata[i] ? v_min : idata[i];
		}
		minv = v_min;
		maxv = v_max;
		return true;
	}

	bool RasterBandImpl::GetMinMaxValue(double& minv, double& maxv)
	{
		double *idata = (double*)GetData();

		int count = m_pRaster->GetWidth() * m_pRaster->GetHeight();
		double v_min = AUGE_DOUBLE_MAX;
		double v_max = AUGE_DOUBLE_MIN;
		for(int i=0; i<count; i++)
		{
			v_min = v_min < idata[i] ? v_min : idata[i];
			v_max = v_max > idata[i] ? v_min : idata[i];
		}
		minv = v_min;
		maxv = v_max;
		return true;
	}

	void RasterBandImpl::Create(GDALRasterBand*	poRasterBand, RasterImpl* pRaster)
	{
		m_pRaster = pRaster;
		m_poRasterBand = poRasterBand;
		m_pixel_size = GetPixelSize(poRasterBand->GetRasterDataType());
		m_data_size = poRasterBand->GetXSize() * poRasterBand->GetYSize() * m_pixel_size;
	}

	void RasterBandImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	g_uint RasterBandImpl::GetPixelSize(GDALDataType type)
	{
		g_uint pixel_size = 1;
		switch(type)
		{
		case GDT_Byte:
			pixel_size = 1;
			break;
		case GDT_UInt16:
		case GDT_Int16:
			pixel_size = 2;
			break;
		case GDT_UInt32:
		case GDT_Int32:
		case GDT_Float32:
			pixel_size = 4;
			break;
		case GDT_Float64:
			pixel_size = 8;
			break;
		case GDT_CInt16:
			pixel_size = 2;
			break;
		case GDT_CInt32:
		case GDT_CFloat32:
			pixel_size = 4;
			break;
		case GDT_CFloat64:
			pixel_size = 8;
			break;
		}
		return pixel_size;
	}

	RESULTCODE RasterBandImpl::Read(void* buffer, g_uint x, g_uint y, g_int width, g_uint height)
	{
		GDALDataType type = m_poRasterBand->GetRasterDataType();
		CPLErr err = m_poRasterBand->RasterIO(	GF_Read, 
												x, y,
												width, height, 
												buffer,
												width, height,
												type, 
												0, 0 );
		if(err!=CE_None)
		{
			return AG_FAILURE;
		}

		return AG_SUCCESS;
	}


}