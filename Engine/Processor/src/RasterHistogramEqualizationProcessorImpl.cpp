#include "RasterHistogramEqualizationProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeUser.h"

#include <fstream>

namespace auge
{
	void print_hist(int *hist, size_t size);

	RasterHistogramEqualizationProcessorImpl::RasterHistogramEqualizationProcessorImpl()
	{
		m_user = 0;
		m_in_raster_path = "/";
		m_out_raster_path = "/";
		m_gray_size = 255;
	}

	RasterHistogramEqualizationProcessorImpl::~RasterHistogramEqualizationProcessorImpl()
	{

	}
	
	void RasterHistogramEqualizationProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void RasterHistogramEqualizationProcessorImpl::SetInputRaster(const char* rasterName)
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

	void RasterHistogramEqualizationProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void RasterHistogramEqualizationProcessorImpl::SetOutputRaster(const char* rasterName)
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
	
	const char*	RasterHistogramEqualizationProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	RasterHistogramEqualizationProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	RasterHistogramEqualizationProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterHistogramEqualizationProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}
	void RasterHistogramEqualizationProcessorImpl::SetInputPath(const char* rasterPath)
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

	void RasterHistogramEqualizationProcessorImpl::SetOutputPath(const char* rasterPath)
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

	const char* RasterHistogramEqualizationProcessorImpl::GetInputRasterPath()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char* RasterHistogramEqualizationProcessorImpl::GetOutputRasterPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	RESULTCODE RasterHistogramEqualizationProcessorImpl::Execute()
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

		//Raster* poutRaster = NULL;
		RasterFolder* poutFolder = NULL;

		pinFolder = pinRasterWorkspace->GetFolder(inRasterPath);
		if(pinFolder==NULL)
		{
			return AG_FAILURE;
		}
		pinRaster = pinFolder->GetRasterDataset()->GetRaster(inRasterName);
		if(pinRaster==NULL)
		{
			pinFolder->Release();
			return AG_FAILURE;
		}

		//g_uint band_count = pinRaster->GetBandCount();
		//if(band_count<3||band_count>4)
		//{
		//	pinFolder->Release();
		//	return AG_FAILURE;
		//}


		if(!HistogramEqualize(pinRaster))
		{
			pinFolder->Release();
			return AG_FAILURE;
		}
		
		poutFolder = poutRasterWorkspace->GetFolder(outRasterPath);
		if(poutFolder==NULL)
		{
			pinFolder->Release();
			return AG_FAILURE;
		}
		RESULTCODE rc = poutFolder->GetRasterDataset()->AddRaster(outRasterName, pinRaster);

		pinRaster->Release();
		poutFolder->Release();
		pinFolder->Release();

		return rc;
	}

	void RasterHistogramEqualizationProcessorImpl::Release()
	{
		delete this;
	}

	bool RasterHistogramEqualizationProcessorImpl::HistogramEqualize(Raster* pRaster)
	{
		g_uint64 w = pRaster->GetWidth();
		g_uint64 h = pRaster->GetHeight();
		g_uint64 size = w * h;

		RasterBand* pBand = NULL;
		g_uint nband = pRaster->GetBandCount();
		for(g_uint i=0; i<nband; i++)
		{
			pBand = pRaster->GetBand(i);
			HistogramEqualize(pBand, w, h, size);
		}
	
		//RasterBand* prBand = pRaster->GetBand(0);
		//RasterBand* pgBand = pRaster->GetBand(1);
		//RasterBand* pbBand = pRaster->GetBand(2);

		//g_uchar* r = (g_uchar*)(prBand->GetData());
		//g_uchar* g = (g_uchar*)(pgBand->GetData());
		//g_uchar* b = (g_uchar*)(pbBand->GetData());

		////Gray = R*0.299 + G*0.587 + B*0.114
		//for(g_uint64 i=0; i<size; i++)
		//{
		//	r[i] = (r[i]*299 + g[i]*587 + b[i]*114) / 1000;
		//}
		//prBand->SetData(r);
		//pgBand->SetData(r);
		//pbBand->SetData(r);

		//free(r);
		//free(g);
		//free(b);


		return true;
	}

	bool RasterHistogramEqualizationProcessorImpl::HistogramEqualize(RasterBand* pBand, g_uint64 w, g_uint64 h, g_uint64 size)
	{
		//g_uint hist[255];
		g_uint* hist = (g_uint*)malloc(sizeof(g_int) * (m_gray_size+1));
		memset(hist, 0, sizeof(g_int)*(m_gray_size+1));

		//g_uint n_hist[255];
		g_uint* n_hist = (g_uint*)malloc(sizeof(g_int) * (m_gray_size+1));
		memset(n_hist, 0, sizeof(g_int)*(m_gray_size+1));

		g_uchar* data = (g_uchar*)(pBand->GetData());
		g_uchar* ptr = data;
		for(g_uint64 i=0; i<size; i++, ptr++)
		{
			hist[*ptr]++;
		}

		HistogramEqualize(n_hist, hist, m_gray_size, size);

		ptr=data;
		for(g_uint64 i=0; i<size; i++, ptr++)
		{
			*ptr = n_hist[*ptr];
		}

		free(hist);
		free(n_hist);
		pBand->SetData(data);
		//free(data);
		return true;
	}

	void RasterHistogramEqualizationProcessorImpl::HistogramEqualize(g_uint* n_hist, g_uint* o_hist, g_uint gray_size, double pixel_count)
	{
		double sum = 0.0;
		for(g_uint i=0; i<gray_size; i++)
		{
			sum += o_hist[i] / pixel_count;
			n_hist[i] =  (g_uint)(sum * 255.0);
		}
	}

	void RasterHistogramEqualizationProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}

	void print_hist(int *hist, size_t size)
	{
		std::ofstream os("g:\\temp\\hist.txt");

		for(int i=0; i<size; i++)
		{
			for(int j=0; j<hist[i]; j++)
			{
				os<<"+";
			}
			os<<std::endl;
		}
		os.close();
	}
}
