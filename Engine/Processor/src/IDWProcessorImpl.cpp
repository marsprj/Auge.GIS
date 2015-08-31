#include "IDWProcessorImpl.h"
#include "Kmean.h"

#include "AugeData.h"
#include "AugeCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeUser.h"

namespace auge
{
	IDWProcessorImpl::IDWProcessorImpl()
	{
		m_user = 0;
		m_cell_size = 0.0f;
		m_z_field.clear();
	}

	IDWProcessorImpl::~IDWProcessorImpl()
	{

	}

	void IDWProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void IDWProcessorImpl::SetInputFeatureClass(const char* className)
	{
		if(className==NULL)
		{
			m_in_class_name.clear();
		}
		else
		{
			m_in_class_name = className;
		}
	}

	void IDWProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void IDWProcessorImpl::SetOutputPath(const char* rasterPath)
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

	void IDWProcessorImpl::SetOutputRaster(const char* rasterName)
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

	void IDWProcessorImpl::SetZField(const char* fname)
	{
		if(fname==NULL)
		{
			m_z_field.clear();
		}
		else
		{
			m_z_field = fname;
		}
	}

	void IDWProcessorImpl::SetExtent(GEnvelope& extent)
	{
		m_extent = extent;
	}

	void IDWProcessorImpl::SetCellSize(float cellSize)
	{
		m_cell_size = cellSize;
	}

	const char* IDWProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char* IDWProcessorImpl::GetInputFatureClass()
	{
		return m_in_class_name.empty() ? NULL : m_in_class_name.c_str();
	}

	const char*	IDWProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char* IDWProcessorImpl::GetOutputRasterPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	const char*	IDWProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	const char* IDWProcessorImpl::GetZField()
	{
		return m_z_field.empty() ? NULL : m_z_field.c_str();
	}

	float IDWProcessorImpl::GetCellSize()
	{
		return m_cell_size;
	}

	GEnvelope& IDWProcessorImpl::GetExtent()
	{
		return m_extent;
	}

	RESULTCODE IDWProcessorImpl::Execute()
	{
		GError	*pError  = augeGetErrorInstance();
		GLogger	*pLogger = augeGetLoggerInstance();

		const char* sourceName_in = GetInputDataSource();
		const char* className_in  = GetInputFatureClass();
		const char* z_field = GetZField();

		const char* sourceName_out= GetOutputDataSource();
		const char* rasterPath_out = GetOutputRasterPath();
		const char* rasterName_out = GetOutputRasterPath();

		if(sourceName_in==NULL)
		{
			return AG_FAILURE;
		}

		if(className_in==NULL)
		{
			return AG_FAILURE;
		}

		if(sourceName_out==NULL)
		{
			return AG_FAILURE;
		}

		if(rasterPath_out==NULL)
		{
			return AG_FAILURE;
		}

		if(rasterName_out==NULL)
		{
			return AG_FAILURE;
		}
		if(m_cell_size<=0)
		{
			return AG_FAILURE;
		}
		if(z_field==NULL)
		{
			return AG_FAILURE;
		}

		FeatureClass		*pinFeatureClass = NULL;
		FeatureClass		*poutFeatureClass= NULL;
		FeatureWorkspace	*pinWorkspace = NULL;
		RasterWorkspace		*poutWorkspace= NULL;
		ConnectionManager	*pConnManager = augeGetConnectionManagerInstance();

		pinWorkspace = dynamic_cast<FeatureWorkspace*>(pConnManager->GetWorkspace(m_user, sourceName_in));
		if(pinWorkspace==NULL)
		{
			return AG_FAILURE;
		}

		poutWorkspace = dynamic_cast<RasterWorkspace*>(pConnManager->NewWorkspace(m_user, sourceName_out));
		if(poutWorkspace==NULL)
		{
			return AG_FAILURE;
		}

		pinFeatureClass = pinWorkspace->OpenFeatureClass(className_in);
		if(pinFeatureClass==NULL)
		{
			poutWorkspace->Release();
			return AG_FAILURE;
		}

		GField* pField = pinFeatureClass->GetFields()->GetGeometryField();
		if(pField==NULL)
		{
			pinFeatureClass->Release();
			poutWorkspace->Release();
			return AG_FAILURE;
		}
		augeGeometryType type = pField->GetGeometryDef()->GeometryType();
		if(type!=augeGTPoint)
		{
			pinFeatureClass->Release();
			poutWorkspace->Release();
			return AG_FAILURE;
		}
	
		pField = pinFeatureClass->GetField(z_field);
		if(pField==NULL)
		{
			pinFeatureClass->Release();
			poutWorkspace->Release();
			return AG_FAILURE;
		}

		RasterFolder* pRasterFolder = NULL;
		pRasterFolder = poutWorkspace->GetFolder(rasterPath_out);
		if(pRasterFolder==NULL)
		{
			pinFeatureClass->Release();
			poutWorkspace->Release();
			return AG_FAILURE;
		}

		Raster* poutRaster = NULL;
		RasterDataset* poutRasterDataset = NULL;
		poutRasterDataset = pRasterFolder->GetRasterDataset();
		poutRaster = poutRasterDataset->GetRaster(rasterName_out);
		if(poutRaster!=NULL)
		{
			pinFeatureClass->Release();
			pinFeatureClass->Release();
			poutRaster->Release();
			pRasterFolder->Release();
			poutWorkspace->Release();
			return AG_FAILURE;
		}

		poutRaster = Interpolate(pinFeatureClass);
		if(poutRaster!=NULL)
		{
			poutRasterDataset->AddRaster(rasterName_out, poutRaster);
		}

		pinFeatureClass->Release();
		pinFeatureClass->Release();
		pRasterFolder->Release();
		poutWorkspace->Release();

		return AG_SUCCESS;
	}

	void IDWProcessorImpl::Release()
	{
		delete this;
	}

	void IDWProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}

	Raster* IDWProcessorImpl::Interpolate(FeatureClass* pinFeatureClass)
	{
		Raster* pRaster = NULL;

		return pRaster;
	}
}
