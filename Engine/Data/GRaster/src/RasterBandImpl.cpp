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

			CPLErr err = m_poRasterBand->RasterIO(GF_Read,
													0,0,
													width,height,
													m_data,
													width,height,
													GDT_Byte,
													1,width);
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
}