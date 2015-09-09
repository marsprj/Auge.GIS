#include "RasterClipProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeFeature.h"
#include "AugeFilter.h"
#include "AugeField.h"
#include "AugeUser.h"

namespace auge
{
	int auge_raster_clip_xx_cmp(const void *a , const void *b)
	{
		return *(double *)a < *(double *)b ? 1 : -1; 
	}

	RasterClipProcessorImpl::RasterClipProcessorImpl()
	{
		m_pFilter = NULL;
		m_user = -1;
	}

	RasterClipProcessorImpl::~RasterClipProcessorImpl()
	{
		if(m_pFilter!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pFilter);
		}
	}

	void RasterClipProcessorImpl::SetUser(g_uint user)
	{

	}

	void RasterClipProcessorImpl::SetInputRasterSource(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_in_raster_source.clear();
		}
		else
		{
			m_in_raster_source = sourceName;
		}
	}

	void RasterClipProcessorImpl::SetInputRaster(const char* rasterName)
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

	void RasterClipProcessorImpl::SetInputPath(const char* rasterPath)
	{
		if(rasterPath==NULL)
		{
			m_in_raster_path = "/";
		}
		else
		{
			if(!strlen(rasterPath))
			{
				m_in_raster_path = "/";
			}
			else
			{
				m_in_raster_path = rasterPath;
			}
		}
	}

	void RasterClipProcessorImpl::SetInputFeatureSource(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_in_feature_source.clear();
		}
		else
		{
			m_in_feature_source = sourceName;
		}
	}

	void RasterClipProcessorImpl::SetInputFeatureClass(const char* className)
	{
		if(className==NULL)
		{
			m_in_feature_name.clear();
		}
		else
		{
			m_in_feature_name = className;
		}
	}

	void RasterClipProcessorImpl::SetInputFilter(GFilter* pFilter)
	{

	}

	void RasterClipProcessorImpl::SetOutpuRasteraSource(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_out_raster_source.clear();
		}
		else
		{
			m_out_raster_source = sourceName;
		}
	}

	void RasterClipProcessorImpl::SetOutputRaster(const char* rasterName)
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

	void RasterClipProcessorImpl::SetOutputPath(const char* rasterPath)
	{
		if(rasterPath==NULL)
		{
			m_out_raster_path = "/";
		}
		else
		{
			if(!strlen(rasterPath))
			{
				m_out_raster_path = "/";
			}
			else
			{
				m_out_raster_path = rasterPath;
			}
		}
	}

	const char*	RasterClipProcessorImpl::GetInputRasterSource()
	{
		return m_in_raster_source.empty() ? NULL : m_in_raster_source.c_str();
	}

	const char*	RasterClipProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	RasterClipProcessorImpl::GetOutputRasterSource()
	{
		return m_out_raster_source.empty() ? NULL : m_out_raster_source.c_str();
	}

	const char*	RasterClipProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	const char* RasterClipProcessorImpl::GetInputRasterPath()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char* RasterClipProcessorImpl::GetOutputRasterPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	const char*	RasterClipProcessorImpl::GetInputFeatureSource()
	{
		return m_in_feature_source.empty() ? NULL : m_in_feature_source.c_str();
	}

	const char*	RasterClipProcessorImpl::GetInputFeatureClass()
	{
		return m_in_feature_name.empty() ? NULL : m_in_feature_name.c_str();
	}

	GFilter* RasterClipProcessorImpl::GetInputFilter()
	{
		return m_pFilter;
	}

	RESULTCODE RasterClipProcessorImpl::Execute()
	{
		const char* inRasterSource = GetInputRasterSource();
		const char* inRasterName = GetInputRaster();
		const char* inRasterPath = GetInputRasterPath();

		const char* inFeatureSource = GetInputFeatureSource();
		const char* inClassName = GetInputFeatureClass();

		const char* outRasterSource = GetOutputRasterSource();
		const char* outRasterName = GetOutputRaster();
		const char* outRasterPath = GetOutputRasterPath();

		GFilter* pFilter = GetInputFilter();

		Workspace* pWorkspace = NULL;
		RasterWorkspace* pinRasterWorkspace = NULL;
		RasterWorkspace* poutRasterWorkspace = NULL;
		FeatureWorkspace* pinFeatureWorkspace = NULL;
		FeatureClass* pinFeatureClass = NULL;

		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = pConnManager->GetWorkspace(m_user, inRasterSource);
		if(pWorkspace==NULL)
		{
			return AG_FAILURE;
		}
		pinRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

		pWorkspace = pConnManager->GetWorkspace(m_user, inFeatureSource);
		if(pWorkspace==NULL)
		{
			return AG_FAILURE;
		}
		pinFeatureWorkspace = dynamic_cast<FeatureWorkspace*>(pWorkspace);
		pinFeatureClass = pinFeatureWorkspace->OpenFeatureClass(inClassName);
		if(pinFeatureClass==NULL)
		{
			return AG_FAILURE;
		}

		pWorkspace = pConnManager->GetWorkspace(m_user, outRasterSource);
		if(pWorkspace==NULL)
		{
			pinFeatureClass->Release();
			return AG_FAILURE;
		}
		poutRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

		Raster* pinRaster = NULL;
		RasterFolder* pinFolder = NULL;

		//Raster* poutRaster = NULL;
		RasterFolder* poutFolder = NULL;

		pinFolder = pinRasterWorkspace->GetFolder(inRasterPath);
		if(pinFolder==NULL)
		{
			pinFeatureClass->Release();
			return AG_FAILURE;
		}
		pinRaster = pinFolder->GetRasterDataset()->GetRaster(inRasterName);
		if(pinRaster==NULL)
		{
			pinFeatureClass->Release();
			pinFolder->Release();
			return AG_FAILURE;
		}

		GEnvelope r_extent, f_extent;
		r_extent = pinRaster->GetExtent();
		//f_extent = pinFeatureClass->GetExtent(pFilter);
		f_extent = pinFeatureClass->GetExtent();

		if(!f_extent.Intersects(r_extent))
		{
			pinFeatureClass->Release();
			pinRaster->Release();
			poutFolder->Release();
			pinFolder->Release();

			return AG_FAILURE;
		}

		GEnvelope o_extent;
		o_extent = f_extent.Intersect(r_extent);

		Raster* poutRaster = NULL;
		poutRaster = Clip(pinRaster, o_extent, pinFeatureClass, pFilter);

		pinFeatureClass->Release();
		pinRaster->Release();
		poutFolder->Release();
		pinFolder->Release();

		return AG_SUCCESS;
	}

	void RasterClipProcessorImpl::Release()
	{
		delete this;
	}

	Raster* RasterClipProcessorImpl::Clip(Raster* pinRaster, GEnvelope& extent, FeatureClass* pinFeatureClass, GFilter* pFilter)
	{
		Raster* poutRaster = NULL;
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();

		poutRaster = pRasterFactory->CreateRaster("", extent, pinRaster);
		if(poutRaster!=NULL)
		{  
			return NULL;
		}

		Geometry* pGeometry = NULL;
		Feature* pFeature = NULL;
		FeatureCursor* pCursor = NULL;
		GQuery* pQuery = NULL;
		FilterFactory* pFilterFactory = augeGetFilterFactoryInstance();
		pQuery = pFilterFactory->CreateQuery();
		pQuery->AddSubField(pinFeatureClass->GetFields()->GetGeometryField()->GetName());
		pQuery->SetFilter(pFilter);
		pFilter->AddRef();

		pCursor = pinFeatureClass->Query(pQuery);
		while((pFeature=pCursor->NextFeature())!=NULL)
		{
			pGeometry = pFeature->GetGeometry();
			if(pGeometry!=NULL)
			{
				//Clip(poutRaster, pinRaster, extent, pGeometry);
			}

			pFeature->Release();
		}

		pCursor->Release();
		pQuery->Release();

		//augePixelType type = pinRaster->GetPixelType();
		//switch(type)
		//{
		//case augePixelByte:
		//	break;
		//case augePixelInt16:
		//case augePixelUInt16:
		//	break;
		//case augePixelInt32:
		//case augePixelUInt32:
		//	break;
		//case augePixelFloat32:
		//	break;
		//case augePixelDouble:
		//	break;
		//}

		return poutRaster;
	}

	RESULTCODE RasterClipProcessorImpl::Clip(Raster* poutRaster, Raster* pinRaster, GEnvelope& extent, Geometry* pGeometry)
	{
		GRect rect;
		pinRaster->GetRasterRect(rect, extent);

		for(g_uint i=rect.m_ymin; i<rect.m_ymax; i++)
		{
			Scan(i, poutRaster, pinRaster, rect, extent, pGeometry);
		}

		return AG_SUCCESS;
	}

	RESULTCODE RasterClipProcessorImpl::Scan(g_uint scanline, Raster* poutRaster, Raster* pinRaster, GRect& rect, GEnvelope& extent, Geometry* pGeometry)
	{
		double mapX, mapY;
		pinRaster->GetMapPosition(0, scanline, mapX, mapY);

		g_uint xc = 0;
		double xx[255];
		memset(xx, 0, 255*sizeof(double));
		
		ComputeScanlineIntersects(xx, xc, mapY, extent, pGeometry);
		if(xc==0)
		{
			return AG_FAILURE;
		}
		qsort(xx, xc, sizeof(double), auge_raster_clip_xx_cmp);
		g_uint px[255];
		memset(px, 0, 255*sizeof(g_uint));
		g_int rx,ry;
		for(g_uint i=0; i<xc; i++)
		{
			pinRaster->GetRasterPosition(xx[i], mapY, rx, ry);
			px[i] = rx;
		}

		g_uint sx,ex;
		for(g_uint i=0; i<xc; i+=2)
		{
			sx = px[i];	ex=px[i+1];
			if(sx>rect.m_xmax)
			{
				break;
			}
			if(ex<rect.m_xmin)
			{
				continue;
			}

			sx = sx > rect.m_xmin ? sx : rect.m_xmin;
			ex = ex < rect.m_xmax ? ex : rect.m_xmax;

			ClipScanLine(poutRaster, pinRaster, sx, ex, scanline);
		}

		return AG_SUCCESS;
	}

	RESULTCODE RasterClipProcessorImpl::ComputeScanlineIntersects(double* xx, g_uint& xc, double mapY, GEnvelope& extent, Geometry* pGeometry)
	{
		xc = 0;

		augeGeometryType geoType = pGeometry->GeometryType();
		switch(geoType)
		{
		case augeGTPolygon:
			ComputeScanlineIntersects(xx, xc, mapY, extent, (WKBPolygon*)pGeometry->AsBinary());
			break;
		case augeGTMultiPolygon:
			ComputeScanlineIntersects(xx, xc, mapY, extent, (WKBMultiPolygon*)pGeometry->AsBinary());
			break;
		}

		return AG_SUCCESS;
	}

	RESULTCODE RasterClipProcessorImpl::ComputeScanlineIntersects(double* xx, g_uint& xc, double mapY, GEnvelope& extent, WKBPolygon* pWKBPolygon)
	{
		int i=0, j=0;
		int numPoints = 0;
		int numRings  = pWKBPolygon->numRings;
		auge::LinearRing* pLinearRing = NULL;
		auge::Point *pt0 = NULL;
		auge::Point *pt1 = NULL;
		double xmax;
		double ymin, ymax;
		double v0, v1;

		pLinearRing = (auge::LinearRing*)(&(pWKBPolygon->rings[0]));
		for(i=0; i<numRings; i++)
		{
			numPoints = pLinearRing->numPoints;
			pt0 = (auge::Point*)(&(pLinearRing->points[0]));
			pt1 = pt0 + 1;

			for(j=1; j<numPoints; j++, pt0++, pt1++)
			{
				v0 = mapY - pt0->y;
				v1 = mapY - pt1->y;
				if((v0==0.0) && (v1==0.0))
				{	//horizonal segment
					//counter++;
				}
				else
				{
					ymin = g_min(pt0->y, pt1->y);
					ymax = g_max(pt0->y, pt1->y);
					v0 = mapY - ymin;
					v1 = mapY - ymax;

					if( v0>0.0 && v1<0.0)
					{
						// x locates between y0 and y1, then x may be accross the segment [pt0, pt1];
						double s1, s2;
						double height = 0.0;
						s1 = mapY - pt0->y;
						s2 = pt1->y - mapY;
						if(fabs(s1)<fabs(s2))
						{
							xx[xc++] = (pt0->x-pt1->x) * ((mapY-pt1->y) / (pt0->y-pt1->y)) + pt1->x;
						}
						else
						{
							double xxx = (pt1->x-pt0->x) * (mapY-pt0->y) / (pt1->y-pt0->y) + pt0->x;
							xx[xc++] = xxx;
						}
					}
				}
			}

			pLinearRing = (auge::LinearRing*)(pt1);
		}

		return AG_SUCCESS;
	}

	RESULTCODE RasterClipProcessorImpl::ComputeScanlineIntersects(double* xx, g_uint& xc, double mapY, GEnvelope& extent, WKBMultiPolygon* pWKBMultiPolygon)
	{
		auge::Point* pt0 = NULL;
		auge::Point* pt1 = NULL; 
		double xmax;
		double ymin, ymax;
		double v0, v1;

		int numPolygons = pWKBMultiPolygon->numPolygons;
		WKBPolygon* pWKBPolygon = &(pWKBMultiPolygon->polygons[0]);
		for(int i=0; i<numPolygons; i++)
		{
			int numRings = pWKBPolygon->numRings;
			LinearRing* pLinearRing = &(pWKBPolygon->rings[0]);
			for(int j=0; j<numRings; j++)
			{	
				int numPoints = pLinearRing->numPoints;
				pt0 = &(pLinearRing->points[0]);
				pt1 = pt0 + 1;
				for(int k=1; k<numPoints; k++, pt0++, pt1++)
				{
					v0 = mapY - pt0->y;
					v1 = mapY - pt1->y;
					if((v0==0.0) && (v1==0.0))
					{	//horizonal segment

					}
					else
					{
						ymin = g_min(pt0->y, pt1->y);
						ymax = g_max(pt0->y, pt1->y);
						v0 = mapY - ymin;
						v1 = mapY - ymax;

						if( v0>0.0 && v1<0.0)
						{
							// x locates between y0 and y1, then x may be accross the segment [pt0, pt1];
							
							double s1, s2;
							double height = 0.0;
							s1 = mapY - pt0->y;
							s2 = pt1->y - mapY;
							if(fabs(s1)<fabs(s2))
							{
								xx[xc++] = (pt0->x-pt1->x) * ((mapY-pt1->y) / (pt0->y-pt1->y)) + pt1->x;
							}
							else
							{
								xx[xc++] = (pt1->x-pt0->x) * (mapY-pt0->y) / (pt1->y-pt0->y) + pt0->x;
							}
						}
					}
				}
				pLinearRing = (LinearRing*)pt1;
			}
			pWKBPolygon = (WKBPolygon*)pt1;
		}

		return AG_SUCCESS;
	}

	RESULTCODE RasterClipProcessorImpl::ClipScanLine(Raster* poutRaster, Raster* pinRaster, g_uint sx, g_uint ex, g_uint y)
	{
		augePixelType type = pinRaster->GetPixelType();
		switch(type)
		{
		case augePixelByte:
			//ClipScanLine_Byte(pinRaster, pinRaster, sx, ex, y);
			break;
		case augePixelUInt16:
		case augePixelInt16:
			ClipScanLine_Int16(pinRaster, pinRaster, sx, ex, y);
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

	RESULTCODE RasterClipProcessorImpl::ClipScanLine_Int16(Raster* poutRaster, Raster* pinRaster, g_uint sx, g_uint ex, g_uint y)
	{
		RasterBand* pinBand = NULL;
		RasterBand* poutBand= NULL;
		g_byte *pinData = NULL;
		g_byte *poutData= NULL;

		g_uint nbands = pinRaster->GetBandCount();
		for(g_uint i=0; i<nbands; i++)
		{
			pinBand = pinRaster->GetBand(i);
			poutBand= poutRaster->GetBand(i);

			//pinData = pinBand->GetData();
		}

		return AG_SUCCESS;
	}
}