#include "RasterImpl.h"
#include "RasterBandImpl.h"
#include "AugeCore.h"

namespace auge
{
	RasterImpl::RasterImpl()
	{
		GDALAllRegister();

		m_srid = 4326;
		m_poDataset = NULL;
		//m_pWorkspace = NULL;
		memset(m_geo_transform, 0, sizeof(double)*6);
	}

	RasterImpl::~RasterImpl()
	{
		Cleanup();
		if(m_poDataset!=NULL)
		{
			GDALClose(m_poDataset);
			m_poDataset = NULL;
		}
	}

	const char* RasterImpl::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void RasterImpl::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		else
		{
			m_name = name;
		}
	}

	const char*	RasterImpl::GetAlias()
	{
		return m_alias.empty() ? NULL : m_alias.c_str();
	}

	void RasterImpl::SetAlias(const char* alias)
	{
		if(alias==NULL)
		{
			m_alias.clear();
		}
		else
		{
			m_alias = alias;
		}
	}

	//augeDataSetType RasterImpl::GetType()
	//{
	//	return augeDataSetRaster;
	//}

	const char* RasterImpl::GetFormat()
	{
		Init();
		return m_format.c_str();
	}

	g_uint RasterImpl::GetWidth()
	{
		Init();
		return m_poDataset->GetRasterXSize();
	}

	g_uint RasterImpl::GetHeight()
	{
		Init();
		return m_poDataset->GetRasterYSize();
	}

	double	RasterImpl::GetResolution_X()
	{
		Init();
		return  m_geo_transform[1];
	}

	double	RasterImpl::GetResolution_Y()
	{
		Init();
		return m_geo_transform[5];
	}

	g_int RasterImpl::GetSRID()
	{
		Init();
		return m_srid;
	}

	const char* RasterImpl::GetSpatialReference()
	{
		Init();
		return m_poDataset->GetProjectionRef();
	}

	g_uint RasterImpl::GetBandCount()
	{
		Init();
		return m_poDataset->GetRasterCount();
	}

	augePixelType RasterImpl::GetPixelType()
	{
		Init();
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
		Init();
		return m_pixel_size;
	}

	GEnvelope& RasterImpl::GetExtent()
	{
		Init();
		return m_extent;
	}

	RasterBand* RasterImpl::GetBand(g_uint i)
	{
		Init();
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
		Init();
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

	bool RasterImpl::GetRasterPosition(double mx, double my, g_int& rx, g_int& ry)
	{
		Init();
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

	bool RasterImpl::GetRasterRect(GRect& rect, GEnvelope& extent)
	{
		Init();
		g_int r_xmin=0, r_ymin=0;
		g_int r_xmax=0, r_ymax=0;
		this->GetRasterPosition(extent.m_xmin, extent.m_ymin, r_xmin, r_ymax);
		this->GetRasterPosition(extent.m_xmax, extent.m_ymax, r_xmax, r_ymin);

		rect.m_xmin = r_xmin < r_xmax ? r_xmin : r_xmax;
		rect.m_xmax = r_xmin > r_xmax ? r_xmin : r_xmax;
		rect.m_ymin = r_ymin < r_ymax ? r_ymin : r_ymax;
		rect.m_ymax = r_ymin > r_ymax ? r_ymin : r_ymax;
		return true;
	}

	bool RasterImpl::GetMinMaxValue(short& minv, short& maxv)
	{
		Init();
		short r_minv = AUGE_INT_MAX;
		short r_maxv = AUGE_INT_MIN;
		short b_minv = AUGE_INT_MAX;
		short b_maxv = AUGE_INT_MIN;

		RasterBandImpl* pBand = NULL;
		std::vector<RasterBandImpl*>::iterator iter;
		for(iter=m_bands.begin(); iter!=m_bands.end(); iter++)
		{
			pBand = (*iter);
			if(pBand->GetMinMaxValue(b_minv, b_maxv))
			{
				r_minv = r_minv < b_minv ? r_minv : b_minv;
				r_maxv = r_maxv > b_maxv ? r_maxv : b_maxv;
			}
		}
		minv = r_minv;
		maxv = r_maxv;

		return true;
	}

	bool RasterImpl::GetMinMaxValue(int& minv, int& maxv)
	{
		Init();
		int r_minv = AUGE_INT_MAX;
		int r_maxv = AUGE_INT_MIN;
		int b_minv = AUGE_INT_MAX;
		int b_maxv = AUGE_INT_MIN;

		RasterBandImpl* pBand = NULL;
		std::vector<RasterBandImpl*>::iterator iter;
		for(iter=m_bands.begin(); iter!=m_bands.end(); iter++)
		{
			pBand = (*iter);
			if(pBand->GetMinMaxValue(b_minv, b_maxv))
			{
				r_minv = r_minv < b_minv ? r_minv : b_minv;
				r_maxv = r_maxv > b_maxv ? r_maxv : b_maxv;
			}
		}
		minv = r_minv;
		maxv = r_maxv;

		return true;
	}

	bool RasterImpl::GetMinMaxValue(double& minv, double& maxv)
	{
		Init();
		return true;
	}

	bool RasterImpl::Create(const char* name, const char* path)
	{
		m_name = name;
		m_alias= name;
		m_path = path;
		m_full_path = path;

		//GDALDataset* poDataset = (GDALDataset*)GDALOpen(path, GA_ReadOnly);	
		//if(poDataset==NULL)
		//{
		//	const char* msg = CPLGetLastErrorMsg();
		//	GError* pError = augeGetErrorInstance();
		//	pError->SetError(msg);
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error(msg, __FILE__, __LINE__);
		//	return false;
		//}
		//m_poDataset = poDataset;

		//// raster format 
		//char ext[AUGE_EXT_MAX] = {0};
		//auge_split_path(path, NULL, NULL, NULL, ext);
		//m_format = ext+1;

		//// extent
		//poDataset->GetGeoTransform(m_geo_transform);
		//double x0 = m_geo_transform[0];
		//double y0 = m_geo_transform[3];
		//double x1 = x0 + m_geo_transform[1] * poDataset->GetRasterXSize();
		//double y1 = y0 + m_geo_transform[5] * poDataset->GetRasterYSize();
		//m_extent.Set(x0,y0,x1,y1);

		//// pixel size
		//m_pixel_size = GetPixelSize((GDALDataType)GetPixelType());

		//// spatial reference
		//const char* proj = poDataset->GetProjectionRef();
		//const char* gproj= poDataset->GetGCPProjection();

		//// set bands
		//int bands = poDataset->GetRasterCount();
		//Cleanup();
		//m_bands.resize(bands);
		//for(int i=0; i<bands; i++)
		//{
		//	m_bands[i] = NULL;
		//}
		
		return true;
	}

	bool RasterImpl::Create(const char* name, GDALDataset* poDataset)
	{
		m_name = name;
		m_alias.clear();
		m_path = "/";
		char ext[AUGE_EXT_MAX];
		memset(ext, 0, AUGE_EXT_MAX);
		auge_split_path(name, NULL, NULL,NULL,ext);
		m_format = ext + 1;

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

	bool RasterImpl::Create(const char* name, GDALDataset* poDataset, GDALDataType pixelType)
	{
		m_name = name;
		m_alias.clear();
		m_path = "/";
		char ext[AUGE_EXT_MAX];
		memset(ext, 0, AUGE_EXT_MAX);
		auge_split_path(name, NULL, NULL,NULL,ext);
		m_format = ext + 1;

		// extent
		int xsize = poDataset->GetRasterXSize();
		int ysize = poDataset->GetRasterYSize();
		poDataset->GetGeoTransform(m_geo_transform);
		double x0 = m_geo_transform[0];
		double y0 = m_geo_transform[3];
		double x1 = x0 + m_geo_transform[1] * poDataset->GetRasterXSize();
		double y1 = y0 + m_geo_transform[5] * poDataset->GetRasterYSize();
		m_extent.Set(x0,y0,x1,y1);

		// pixel size
		m_pixel_size = GetPixelSize(pixelType);

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
		Init();
		//return m_path.empty() ? NULL : m_path.c_str();
		return m_full_path.empty() ? NULL : m_full_path.c_str();
	}

	void RasterImpl::SetPath(const char* path)
	{
		if(path==NULL)
		{
			m_path = "/";
		}
		else
		{
			m_path = path;
		}
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
			if(pBand!=NULL)
			{
				pBand->Release();
			}
		}
		m_bands.clear();
	}

	RESULTCODE RasterImpl::Save(const char* path, const char* format/*=NULL*/)
	{
		Init();
		//if(m_path.empty())
		{
			if(format==NULL)
			{
				char ext[AUGE_EXT_MAX];
				memset(ext, 0, AUGE_EXT_MAX);
				auge_split_path(path, NULL, NULL, NULL, ext);
				format = auge_raster_get_driver(ext + 1);
			}
			GDALDriver* poDriver = GetGDALDriverManager()->GetDriverByName(format);
			if(poDriver==NULL)
			{
				const char* msg = CPLGetLastErrorMsg();			
				augeGetErrorInstance()->SetError(msg);
				augeGetLoggerInstance()->Error(msg,__FILE__,__LINE__);

				return AG_FAILURE;
			}

			// Save Image
			GDALDataset* poDataset = NULL;
			poDataset = poDriver->CreateCopy(path, m_poDataset, FALSE, NULL, NULL, NULL);
			if(poDataset==NULL)
			{
				return AG_FAILURE;
			}
			poDataset->FlushCache();
			GDALClose(m_poDataset);

			m_poDataset = poDataset;
			m_full_path = path;
		}
		return AG_SUCCESS;
	}

	void RasterImpl::SetUUID(const char* uuid)
	{
		m_uuid = uuid;
	}

	const char* RasterImpl::GetUUID()
	{
		return m_uuid.c_str();
	}

	bool RasterImpl::Init()
	{
		if(m_poDataset!=NULL)
		{
			return true;
		}
		GDALDataset* poDataset = (GDALDataset*)GDALOpen(m_path.c_str(), GA_ReadOnly);	
		if(poDataset==NULL)
		{
			const char* msg = CPLGetLastErrorMsg();
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			return false;
		}
		m_poDataset = poDataset;

		// raster format 
		char ext[AUGE_EXT_MAX] = {0};
		auge_split_path(m_path.c_str(), NULL, NULL, NULL, ext);
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
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	const char* auge_raster_get_driver(const char* ext)
	{
		if(g_stricmp(ext,"png")==0)
		{
			return "png";
		}
		else if((g_stricmp(ext,"jpg")==0)||(g_stricmp(ext,"jpeg")==0))
		{
			return "jpeg";
		}
		else if((g_stricmp(ext,"tif")==0)||(g_stricmp(ext,"tiff")==0))
		{
			return "GTiff";
		}
		return "";
	}
}