#include "RasterMosiacProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeFeature.h"
#include "AugeFilter.h"
#include "AugeField.h"
#include "AugeUser.h"

#include <fstream>

namespace auge
{

	RasterMosiacProcessorImpl::RasterMosiacProcessorImpl()
	{
		m_user = -1;
	}

	RasterMosiacProcessorImpl::~RasterMosiacProcessorImpl()
	{
		
	}

	void RasterMosiacProcessorImpl::SetUser(g_uint user)
	{

	}

	void RasterMosiacProcessorImpl::AddInputRaster(const char* sourceName, const char* rasterPath, const char* rasterName)
	{

	}


	void RasterMosiacProcessorImpl::SetOutpuRasteraSource(const char* sourceName)
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

	void RasterMosiacProcessorImpl::SetOutputRaster(const char* rasterName)
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

	void RasterMosiacProcessorImpl::SetOutputPath(const char* rasterPath)
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

	const char*	RasterMosiacProcessorImpl::GetOutputRasterSource()
	{
		return m_out_raster_source.empty() ? NULL : m_out_raster_source.c_str();
	}

	const char*	RasterMosiacProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}
	
	const char* RasterMosiacProcessorImpl::GetOutputRasterPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}


	RESULTCODE RasterMosiacProcessorImpl::Execute()
	{
		//const char* inRasterSource = GetInputRasterSource();
		//const char* inRasterName = GetInputRaster();
		//const char* inRasterPath = GetInputRasterPath();

		//const char* inFeatureSource = GetInputFeatureSource();
		//const char* inClassName = GetInputFeatureClass();

		//const char* outRasterSource = GetOutputRasterSource();
		//const char* outRasterName = GetOutputRaster();
		//const char* outRasterPath = GetOutputRasterPath();

		//GFilter* pFilter = GetInputFilter();

		//Workspace* pWorkspace = NULL;
		//RasterWorkspace* pinRasterWorkspace = NULL;
		//RasterWorkspace* poutRasterWorkspace = NULL;
		//FeatureWorkspace* pinFeatureWorkspace = NULL;
		//FeatureClass* pinFeatureClass = NULL;

		//ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		//pWorkspace = pConnManager->GetWorkspace(m_user, inRasterSource);
		//if(pWorkspace==NULL)
		//{
		//	return AG_FAILURE;
		//}
		//pinRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

		//pWorkspace = pConnManager->GetWorkspace(m_user, inFeatureSource);
		//if(pWorkspace==NULL)
		//{
		//	return AG_FAILURE;
		//}
		//pinFeatureWorkspace = dynamic_cast<FeatureWorkspace*>(pWorkspace);
		//pinFeatureClass = pinFeatureWorkspace->OpenFeatureClass(inClassName);
		//if(pinFeatureClass==NULL)
		//{
		//	return AG_FAILURE;
		//}

		//pWorkspace = pConnManager->GetWorkspace(m_user, outRasterSource);
		//if(pWorkspace==NULL)
		//{
		//	pinFeatureClass->Release();
		//	return AG_FAILURE;
		//}
		//poutRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

		//Raster* pinRaster = NULL;
		//RasterFolder* pinFolder = NULL;

		////Raster* poutRaster = NULL;
		//RasterFolder* poutFolder = NULL;

		//pinFolder = pinRasterWorkspace->GetFolder(inRasterPath);
		//if(pinFolder==NULL)
		//{
		//	pinFeatureClass->Release();
		//	return AG_FAILURE;
		//}
		//pinRaster = pinFolder->GetRasterDataset()->GetRaster(inRasterName);
		//if(pinRaster==NULL)
		//{
		//	pinFeatureClass->Release();
		//	pinFolder->Release();
		//	return AG_FAILURE;
		//}

		//GEnvelope r_extent, f_extent;
		//r_extent = pinRaster->GetExtent();
		////f_extent = pinFeatureClass->GetExtent(pFilter);
		//f_extent = pinFeatureClass->GetExtent();

		//if(!f_extent.Intersects(r_extent))
		//{
		//	pinFeatureClass->Release();
		//	pinRaster->Release();
		//	poutFolder->Release();
		//	pinFolder->Release();

		//	return AG_FAILURE;
		//}

		//GEnvelope o_extent;
		//o_extent = f_extent.Intersect(r_extent);

		//Raster* poutRaster = NULL;
		//poutRaster = Clip(pinRaster, o_extent, pinFeatureClass, pFilter);

		//pinFeatureClass->Release();
		//pinRaster->Release();
		//poutFolder->Release();
		//pinFolder->Release();

		return AG_SUCCESS;
	}

	void RasterMosiacProcessorImpl::Release()
	{
		delete this;
	}


	Raster*	RasterMosiacProcessorImpl::Mosiac(char** rasters, g_uint count, const char* poutRasterPath)
	{
		if(count==0)
		{
			return NULL;
		}

		Raster* pinRaster = NULL;
		Raster* pinRaster0 = NULL;
		Raster* poutRaster = NULL;
		Raster** ppinRasters = new Raster*[count];
		memset(ppinRasters, 0, sizeof(Raster*)*count);

		RasterIO* io = augeGetRasterIOInstance();
		const char* rpath = NULL;
		for(g_uint i=0; i<count; i++)
		{
			rpath = rasters[i];
			if(rpath!=NULL)
			{
				pinRaster = io->Read(rpath);
				ppinRasters[i] = pinRaster;
			}
		}

		if(!MosiacCheck(ppinRasters, count))
		{
			CleanupRasters(ppinRasters, count);
			return NULL;
		}

		pinRaster0 = GetRaster0(ppinRasters, count);
		if(pinRaster0==NULL)
		{
			CleanupRasters(ppinRasters, count);
			return NULL;
		}

		char uuid[AUGE_NAME_MAX];
		auge_generate_uuid(uuid, AUGE_NAME_MAX);
		g_uint srid  = pinRaster0->GetSRID();
		g_uint bands = pinRaster0->GetBandCount();
		const char* format = pinRaster0->GetFormat();
		double resolution_x = pinRaster0->GetResolution_X();
		double resolution_y = pinRaster0->GetResolution_Y();
		augePixelType pixelType = pinRaster0->GetPixelType();
		const char* spatialref = pinRaster0->GetSpatialReference();

		GEnvelope extent;
		if(!ComputeMosiacExtent(extent, ppinRasters, count))
		{
			CleanupRasters(ppinRasters, count);
			return NULL;
		}

		g_uint width, height;
		width = ceil(extent.GetWidth() / fabs(resolution_x));
		height= ceil(extent.GetHeight() / fabs(resolution_y));

		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();

		{
			poutRaster = pRasterFactory->CreateRaster("", pixelType, bands, extent, width, height,spatialref);

			g_uint64 pixelSize = (g_uint64)(pinRaster0->GetPixelSize());
			g_uint64 buffer_size = width * height * pixelSize;
			void* buffer = malloc(buffer_size);

			g_uint bands = poutRaster->GetBandCount();
			for(g_uint i=0; i<bands;i++)
			{	
				memset(buffer, 0, buffer_size);

				RasterBand* poutBand = poutRaster->GetBand(i);
				//for(int j=0; j<count; j++)
				for(int j=0; j<1; j++)
				{
					pinRaster = ppinRasters[j];

					RasterBand* pinBand = pinRaster->GetBand(i);
					CopyBandMem(poutBand, pinBand, poutRaster, buffer);
				}
				poutBand->SetData(buffer);
			}

			poutRaster->Save(poutRasterPath);

			free(buffer);
		}
		

		{
		//	//·Ö¿éMosiac
		//	poutRaster = pRasterFactory->CreateRaster("", poutRasterPath,"gtiff", pixelType, bands, extent, width, height,spatialref);
		//	//CreateRaster(const char* name, const char* path, const char* format, augePixelType pixelType, g_uint bands, GEnvelope& extent, g_uint width, g_uint height, const char*  spatialRef);
		//	if(poutRaster==NULL)
		//	{
		//		CleanupRasters(ppinRasters, count);
		//		return NULL;
		//	}
		//	for(g_uint i=0; i<count;i++)
		//		//for(g_uint i=0; i<1;i++)
		//		//for(g_uint i=1; i<2;i++)
		//	{
		//		pinRaster = ppinRasters[i];
		//		Mosiac(poutRaster, pinRaster);
		//	}
		}
		
		
		CleanupRasters(ppinRasters, count);

		return poutRaster;
	}

	bool RasterMosiacProcessorImpl::ComputeMosiacExtent(GEnvelope& mextent, Raster** ppinRasters, g_uint count)
	{
		GEnvelope rect;
		Raster* pRaster = NULL;
		for(g_uint i=0; i<count; i++)
		{
			pRaster = ppinRasters[i];
			rect = pRaster->GetExtent();
			if(mextent.IsValid())
			{
				if(rect.IsValid())
				{
					mextent.Union(rect);
				}
			}
			else
			{
				mextent = rect;
			}
		}

		return mextent.IsValid();
	}

	bool RasterMosiacProcessorImpl::MosiacCheck(Raster** ppinRasters, g_uint count)
	{
		return true;
	}

	void RasterMosiacProcessorImpl::CleanupRasters(Raster** ppinRasters, g_uint count)
	{
		if(ppinRasters!=NULL)
		{
			Raster* pRaster = NULL;
			for(g_uint i=0; i<count; i++)
			{
				pRaster = ppinRasters[i];
				if(pRaster!=NULL)
				{
					AUGE_SAFE_RELEASE(pRaster);
				}
			}
		}
		delete[] ppinRasters;
		ppinRasters = NULL;
	}

	augePixelType RasterMosiacProcessorImpl::GetPixelType(Raster** ppRaster, g_uint count)
	{
		Raster* pRaster = NULL;

		for(g_uint i=0; i<count; i++)
		{
			pRaster = ppRaster[i];
			if(pRaster!=NULL)
			{
				return pRaster->GetPixelType();
			}
		}

		return augePixelUnknown;
	}

	Raster*	RasterMosiacProcessorImpl::GetRaster0(Raster** ppRaster, g_uint count)
	{
		Raster* pRaster = NULL;

		for(g_uint i=0; i<count; i++)
		{
			pRaster = ppRaster[i];
			if(pRaster!=NULL)
			{
				return pRaster;
			}
		}

		return NULL;
	}

	RESULTCODE RasterMosiacProcessorImpl::Mosiac(Raster* poutRaster, Raster* pinRaster)
	{
		RasterBand* pinBand = NULL;
		RasterBand* poutBand = NULL;
		g_uint bands = poutRaster->GetBandCount();
		for(g_uint i=0; i<bands; i++)
		{
			pinBand = pinRaster->GetBand(i);
			poutBand = poutRaster->GetBand(i);

			CopyBand(poutRaster, poutBand, pinBand);
		}
		return AG_SUCCESS;
	}

	RESULTCODE RasterMosiacProcessorImpl::CopyBand(Raster* poutRaster, RasterBand* poutBand, RasterBand* pinBand)
	{
		GEnvelope extent = pinBand->GetExtent();

		int xmin, ymin, xmax, ymax;
		poutRaster->GetRasterPosition(extent.m_xmin, extent.m_ymax, xmin, ymin);
		poutRaster->GetRasterPosition(extent.m_xmax, extent.m_ymin, xmax, ymax);

		xmax = xmax < poutRaster->GetWidth() ? xmax : poutRaster->GetWidth();
		ymax = ymax < poutRaster->GetHeight() ? ymax : poutRaster->GetHeight();

		int width = xmax - xmin;
		int height= ymax - ymin;

		width = width < pinBand->GetWidth() ? width : pinBand->GetWidth();
		height= height< pinBand->GetHeight() ? height : pinBand->GetHeight();

		int pixelSize = poutRaster->GetPixelSize();
		int dataSize =  pixelSize * width * height;
		void* data = malloc(dataSize);
		memset(data, 0, dataSize);
		GRect rect(0,0,width,height);
		//pinBand->GetData();
		pinBand->GetData(data, rect);
		
		g_int16* buffer = (g_int16*)data;
		RESULTCODE rc = poutBand->Write(data, xmin, ymin, width, height);

		{
			//g_int16* ptr =buffer;
			//std::ofstream os("h:\\temp\\result\\04.txt");
			//int i,j;
			//for(i=0; i<height; i++)
			//{
			//	for(j=0; j<width; j++, ptr++)
			//	{
			//		os<<*ptr<<"\t";
			//	}
			//	os<<std::endl;

			//}
			//os.flush();
			//os.close();
		}

		{
			//memset(data, 0, dataSize);
			//poutBand->Read(data, xmin, ymin, width, height);
			//g_int16* ptr = (g_int16*)data;
			//std::ofstream os("h:\\temp\\result\\result.txt");
			//int i,j;
			//for(i=0; i<height; i++)
			//{
			//	for(j=0; j<width; j++, ptr++)
			//	{
			//		os<<*ptr<<"\t";
			//	}
			//	os<<std::endl;

			//}
			//os.flush();
			//os.close();
		}


		free(data);
		return AG_SUCCESS;
	}

	RESULTCODE RasterMosiacProcessorImpl::MosiacMem(Raster* pinRaster, Raster* poutRaster, void* buffer, g_uint64 buffer_size)
	{
		RasterBand* pinBand = NULL;
		RasterBand* poutBand = NULL;

		memset(buffer, 0, buffer_size);

		g_uint bands = poutRaster->GetBandCount();
		for(g_uint i=0; i<bands; i++)
		{
			pinBand = pinRaster->GetBand(i);
			poutBand = poutRaster->GetBand(i);

			CopyBandMem(poutBand, pinBand, poutRaster, buffer);
		}
		poutBand->SetData(buffer);
		return AG_SUCCESS;
	}

	RESULTCODE RasterMosiacProcessorImpl::CopyBandMem(RasterBand* poutBand, RasterBand* pinBand, Raster* poutRaster, void* buffer)
	{
		GEnvelope extent = pinBand->GetExtent();

		int xmin, ymin, xmax, ymax;
		poutRaster->GetRasterPosition(extent.m_xmin, extent.m_ymax, xmin, ymin);
		poutRaster->GetRasterPosition(extent.m_xmax, extent.m_ymin, xmax, ymax);

		xmax = xmax < poutRaster->GetWidth() ? xmax : poutRaster->GetWidth();
		ymax = ymax < poutRaster->GetHeight() ? ymax : poutRaster->GetHeight();		

		g_uint64 width = poutBand->GetWidth();
		g_uint64 offset = ((g_uint64)ymin) * poutBand->GetWidth() + xmin;
		char* ptr = (char*)buffer + offset;
		int rwidth = width * poutRaster->GetPixelSize();

		int block_width = xmax - xmin;
		int block_height= ymax - ymin;
		block_width = block_width < pinBand->GetWidth() ? block_width : pinBand->GetWidth();
		block_height= block_height< pinBand->GetHeight() ? block_height : pinBand->GetHeight();
		g_uint64 block_size = (g_uint64)block_width * block_height * poutRaster->GetPixelSize();
		char* data = (char*)malloc(block_size);
		char* dptr = data;
		int dwidth = block_width * poutRaster->GetPixelSize();

		GRect rect(0,0,block_width,block_height);
		pinBand->GetData();
		pinBand->GetData(data, rect);

		for(int i=0; i<block_height; i++, ptr+=rwidth, dptr+dwidth)
		{
			memcpy(ptr, dptr, dwidth);
		}

		free(data);

		return AG_SUCCESS;
	}
}