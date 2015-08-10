#include "RasterPenEffectProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeUser.h"
#include <math.h>

namespace auge
{
	double color_diff(double r_0, double g_0, double b_0, double r_1, double g_1, double b_1);

	RasterPenEffectProcessorImpl::RasterPenEffectProcessorImpl()
	{
		m_user = 0;
		m_in_raster_path = "/";
		m_out_raster_path = "/";
		m_t_size = 5;
	}

	RasterPenEffectProcessorImpl::~RasterPenEffectProcessorImpl()
	{

	}
	
	void RasterPenEffectProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void RasterPenEffectProcessorImpl::SetInputRaster(const char* rasterName)
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

	void RasterPenEffectProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void RasterPenEffectProcessorImpl::SetOutputRaster(const char* rasterName)
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
	
	const char*	RasterPenEffectProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	RasterPenEffectProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	RasterPenEffectProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterPenEffectProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}
	void RasterPenEffectProcessorImpl::SetInputPath(const char* rasterPath)
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

	void RasterPenEffectProcessorImpl::SetOutputPath(const char* rasterPath)
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

	const char* RasterPenEffectProcessorImpl::GetInputRasterPath()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char* RasterPenEffectProcessorImpl::GetOutputRasterPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	RESULTCODE RasterPenEffectProcessorImpl::Execute()
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

		g_uint band_count = pinRaster->GetBandCount();
		if(band_count<3||band_count>4)
		{
			pinRaster->Release();
			pinFolder->Release();
			return AG_FAILURE;
		}


		Raster* poutRaster = PenEffect(pinRaster);
		if(poutRaster==NULL)
		{
			poutRaster->Release();
			pinRaster->Release();
			pinFolder->Release();
			return AG_FAILURE;
		}
		
		poutFolder = poutRasterWorkspace->GetFolder(outRasterPath);
		if(poutFolder==NULL)
		{
			pinRaster->Release();
			pinFolder->Release();
			return AG_FAILURE;
		}
		RESULTCODE rc = poutFolder->GetRasterDataset()->AddRaster(outRasterName, poutRaster);

		pinRaster->Release();
		poutRaster->Release();
		poutFolder->Release();
		pinFolder->Release();

		return rc;
	}

	void RasterPenEffectProcessorImpl::Release()
	{
		delete this;
	}

	Raster* RasterPenEffectProcessorImpl::PenEffect(Raster* pinRaster)
	{
		Raster* poutRaster = NULL;
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();
		poutRaster = pRasterFactory->CreateRaster("", pinRaster->GetExtent(), pinRaster);
		if(poutRaster==NULL)
		{
			return NULL;
		}
		
		g_uint64 w = pinRaster->GetWidth();
		g_uint64 h = pinRaster->GetHeight();
		g_uint64 size = w * h;

		g_int64 t_w = m_t_size >> 1;
		g_int64 t_h = m_t_size >> 1;
		g_uint64 t_square = m_t_size * m_t_size;
	
		RasterBand* pi_rBand = pinRaster->GetBand(0);
		RasterBand* pi_gBand = pinRaster->GetBand(1);
		RasterBand* pi_bBand = pinRaster->GetBand(2);
		g_uchar* r_i = (g_uchar*)(pi_rBand->GetData());
		g_uchar* g_i = (g_uchar*)(pi_gBand->GetData());
		g_uchar* b_i = (g_uchar*)(pi_bBand->GetData());

		RasterBand* po_rBand = poutRaster->GetBand(0);
		RasterBand* po_gBand = poutRaster->GetBand(1);
		RasterBand* po_bBand = poutRaster->GetBand(2);
		g_uchar* r_o = (g_uchar*)(po_rBand->GetData());
		g_uchar* g_o = (g_uchar*)(po_gBand->GetData());
		g_uchar* b_o = (g_uchar*)(po_bBand->GetData());
		reset_band_255(r_o, size);
		reset_band_255(g_o, size);
		reset_band_255(b_o, size);

		g_uchar br_i, bg_i, bb_i;

		g_uint64 i=0, j=0;
		g_uint64 r=0, c=0;
		for(r=0; r<h; r++)
		{
			for(c=0; c<w; c++, i++)
			{
				br_i = r_i[i];
				bg_i = g_i[i];
				bb_i = b_i[i];

				double moment = 0.0;
				g_int64 r_s=0, c_s=0;
				g_uint64 new_x, new_y;
				g_uint64 new_i;
				g_uchar new_r, new_g, new_b;
				for(r_s=-t_h; r_s<t_h; r_s++)
				{
					for(c_s=-t_w; c_s<t_w; c_s++)
					{
						new_x = r + r_s;
						new_y = c + c_s;

						new_x = new_x < 0 ? 0 : new_x;
						new_y = new_y < 0 ? 0 : new_y;
						new_x = new_x > w ? w-1 : new_x;
						new_y = new_y > h ? h-1 : new_y;

						new_i = new_y * w + new_x;
						new_r = r_i[new_i];
						new_g = g_i[new_i];
						new_b = b_i[new_i];
						moment += color_diff(br_i, bg_i, bb_i, new_r, new_g, new_b);
					}	
				}
				g_uchar value = 255.0 * moment / t_square;
				r_o[i] = value;
				g_o[i] = value;
				b_o[i] = value;
			}
		}
		po_rBand->SetData(r_o);
		po_gBand->SetData(g_o);
		po_bBand->SetData(b_o);

		return poutRaster;
	}

	void RasterPenEffectProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}

	void RasterPenEffectProcessorImpl::reset_band_255(g_uchar* data, g_uint64 size)
	{
		g_uchar* ptr = data;
		for(g_uint i=size; i; i--, ptr++)
		{
			*ptr = 255;
		}
	}

	double color_diff(double r_0, double g_0, double b_0, double r_1, double g_1, double b_1)
	{
		return sqrt(pow((r_0-r_1),2.0) + pow((g_0-g_1),2.0) + pow((b_0-b_1),2.0));
	}
}
