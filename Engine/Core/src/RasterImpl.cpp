#include "RasterImpl.h"
#include "RasterBandImpl.h"
#include "AugeCore.h"

namespace auge
{
	RasterImpl::RasterImpl()
	{
		m_srid = 4326;
		m_poDataset = NULL;
		//m_pWorkspace = NULL;
		memset(m_geo_transform, 0, sizeof(double)*6);
	}

	RasterImpl::~RasterImpl()
	{
		if(m_poDataset!=NULL)
		{
			GDALClose(m_poDataset);
			m_poDataset = NULL;
		}
	}

	const char* RasterImpl::GetName()
	{
		return m_name.c_str();
	}

	const char*	RasterImpl::GetAlias()
	{
		return m_alias.c_str();
	}

	//augeDataSetType RasterImpl::GetType()
	//{
	//	return augeDataSetRaster;
	//}

	const char* RasterImpl::GetFormat()
	{
		return m_format.c_str();
	}

	g_uint RasterImpl::GetWidth()
	{
		return m_poDataset->GetRasterXSize();
	}

	g_uint RasterImpl::GetHeight()
	{
		return m_poDataset->GetRasterYSize();
	}

	g_int RasterImpl::GetSRID()
	{
		return m_srid;
	}

	g_uint RasterImpl::GetBandCount()
	{
		return m_poDataset->GetRasterCount();
	}

	augePixelType RasterImpl::GetPixelType()
	{
		if(m_poDataset==NULL)
		{
			return augePixelUnknown;
		}

		int nBands = m_poDataset->GetRasterCount();
		if(nBands==0)
		{
			return augePixelUnknown;
		}

		GDALRasterBand* poBand = m_poDataset->GetRasterBand(1);
		GDALDataType type = poBand->GetRasterDataType();
		return (augePixelType)type;
	}

	g_uint RasterImpl::GetPixelSize()
	{
		return m_pixel_size;
	}

	GEnvelope& RasterImpl::GetExtent()
	{
		return m_extent;
	}

	RasterBand* RasterImpl::GetBand(g_uint i)
	{
		if(i>=m_bands.size())
		{
			return NULL;
		}

		RasterBandImpl* pBand = NULL;
		pBand = m_bands[i];
		if(pBand)
		{
			return pBand;
		}

		GDALRasterBand* poBand = m_poDataset->GetRasterBand(i+1);
		RasterBandImpl* pBandImpl = new RasterBandImpl();
		pBandImpl->Create(poBand, this);
		m_bands[i] = pBandImpl;
		
		return pBandImpl;
	}

	bool RasterImpl::GetMapPosition(g_uint rx, g_uint ry, double& mx, double& my)
	{
		g_uint w = m_poDataset->GetRasterXSize();
		g_uint h = m_poDataset->GetRasterYSize();
		if((rx>w) || (ry>h))
		{
			return false;
		}

		mx = m_extent.m_xmin + m_extent.GetWidth() * ((double)rx / (double)w);
		my = m_extent.m_ymin + m_extent.GetHeight() * ((double)(h-ry) / (double)h);

		return true;
	}

	bool RasterImpl::GetRasterPosition(double mx, double my, g_uint& rx, g_uint& ry)
	{
		if(!m_extent.Contain(mx,my))
		{
			return false;
		}

		g_uint w = m_poDataset->GetRasterXSize();
		g_uint h = m_poDataset->GetRasterYSize();

		double rate_x = (mx - m_extent.m_xmin) / m_extent.GetWidth();
		double rate_y = (my - m_extent.m_ymin) / m_extent.GetHeight();

		rx = w * rate_x;
		ry = h - h * rate_y;

		return true;
	}

	bool RasterImpl::Create(const char* name, const char* path)
	{
		m_name = name;
		m_alias= name;
		m_path = path;

		GDALDataset* poDataset = (GDALDataset*)GDALOpen(path, GA_ReadOnly);	
		if(poDataset==NULL)
		{
			return false;
		}

		// raster format 
		char ext[AUGE_EXT_MAX] = {0};
		auge_split_path(path, NULL, NULL, NULL, ext);
		m_format = ext+1;

		// extent
		poDataset->GetGeoTransform(m_geo_transform);
		double x0 = m_geo_transform[0];
		double y0 = m_geo_transform[3];
		double x1 = x0 + m_geo_transform[1] * poDataset->GetRasterXSize();
		double y1 = y0 + m_geo_transform[5] * poDataset->GetRasterYSize();
		m_extent.Set(x0,y0,x1,y1);

		// pixel size
		m_pixel_size = GetPixelSize((GDALDataType)GetPixelType());

		// spatial reference
		const char* proj = poDataset->GetProjectionRef();
		const char* gproj= poDataset->GetGCPProjection();

		// set bands
		int bands = poDataset->GetRasterCount();
		Cleanup();
		m_bands.resize(bands);
		for(int i=0; i<bands; i++)
		{
			m_bands[i] = NULL;
		}
		m_poDataset = poDataset;
		return true;
	}

	//void RasterImpl::Create(const char* name, GDALDataset* poDataset, WorkspaceRaster* pWorkspace, const char* path)
	//{
	//	m_name = name;
	//	m_path = path;
	//	m_poDataset = poDataset;
	//	m_pWorkspace = pWorkspace;

	//	m_poDataset->GetGeoTransform(m_geo_transform);

	//	double x0 = m_geo_transform[0];
	//	double y0 = m_geo_transform[3];
	//	double x1 = x0 + m_geo_transform[1] * m_poDataset->GetRasterXSize();
	//	double y1 = y0 + m_geo_transform[5] * m_poDataset->GetRasterYSize();
	//	m_extent.Set(x0,y0,x1,y1);

	//	m_pixel_size = GetPixelSize((GDALDataType)GetPixelType());

	//	int bands = poDataset->GetRasterCount();
	//	Cleanup();
	//	m_bands.resize(bands);
	//	for(int i=0; i<bands; i++)
	//	{
	//		m_bands[i] = NULL;
	//	}
	//}

	const char*	RasterImpl::GetPath()
	{
		return m_path.c_str();
	}

	g_uint RasterImpl::GetPixelSize(GDALDataType type)
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

	void RasterImpl::Cleanup()
	{
		RasterBandImpl* pBand = NULL;
		std::vector<RasterBandImpl*>::iterator iter;
		for(iter=m_bands.begin(); iter!=m_bands.end(); iter++)
		{
			pBand = (*iter);
			pBand->Release();
		}
		m_bands.clear();
	}
}