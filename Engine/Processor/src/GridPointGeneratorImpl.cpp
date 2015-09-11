#include "GridPointGeneratorImpl.h"

#include "AugeData.h"
#include "AugeCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeUser.h"

#include <math.h>

namespace auge
{
	GridPointGeneratorImpl::GridPointGeneratorImpl()
	{
		m_user = 0;
		m_user = AUGE_DEFAULT_SRID;
	}

	GridPointGeneratorImpl::~GridPointGeneratorImpl()
	{

	}

	void GridPointGeneratorImpl::SetOutputDataSource(const char* sourceName)
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

	void GridPointGeneratorImpl::SetOutputSRID(g_uint srid)
	{
		m_srid = srid;
	}

	void GridPointGeneratorImpl::SetOutputFeatureClass(const char* className)
	{
		if(className==NULL)
		{
			m_out_class_name.clear();
		}
		else
		{
			m_out_class_name = className;
		}
	}

	void GridPointGeneratorImpl::SetExtent(GEnvelope& extent)
	{
		m_extent = extent;
	}

	void GridPointGeneratorImpl::SetCellSize(float cellSize)
	{
		m_cell_size = cellSize;
	}

	const char*	GridPointGeneratorImpl::GetOutputSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	GridPointGeneratorImpl::GetOutputFatureClass()
	{
		return m_out_class_name.empty() ? NULL : m_out_class_name.c_str();
	}

	float GridPointGeneratorImpl::GetCellSize()
	{
		return m_cell_size;
	}

	GEnvelope& GridPointGeneratorImpl::GetExtent()
	{
		return m_extent;
	}

	RESULTCODE GridPointGeneratorImpl::Execute()
	{
		GError	*pError  = augeGetErrorInstance();
		GLogger	*pLogger = augeGetLoggerInstance();

		const char* sourceName_out= GetOutputSource();
		const char* className_out = GetOutputFatureClass();
		
		if(sourceName_out==NULL)
		{
			return AG_FAILURE;
		}

		if(className_out==NULL)
		{
			return AG_FAILURE;
		}

		FeatureClass		*poutFeatureClass= NULL;
		FeatureWorkspace	*poutWorkspace= NULL;
		ConnectionManager	*pConnManager = augeGetConnectionManagerInstance();
		
		poutWorkspace = dynamic_cast<FeatureWorkspace*>(pConnManager->NewWorkspace(m_user, sourceName_out));
		if(poutWorkspace==NULL)
		{
			return AG_FAILURE;
		}

		g_uint srid_out = m_srid;

		poutFeatureClass = CreateOutputFeatureClass(className_out, srid_out, poutWorkspace);
		if(poutFeatureClass==NULL)
		{
			poutWorkspace->Release();
			return AG_FAILURE;
		}

		RESULTCODE rc = AG_FAILURE;
		rc = Generate(poutFeatureClass);
		poutFeatureClass->Refresh();
		poutFeatureClass->Release();

		if(rc!=AG_SUCCESS)
		{
			poutWorkspace->RemoveFeatureClass(className_out);
		}

		poutWorkspace->Release();

		return AG_SUCCESS;
	}

	void GridPointGeneratorImpl::Release()
	{
		delete this;
	}

	FeatureClass* GridPointGeneratorImpl::CreateOutputFeatureClass(const char* className, g_uint srid, FeatureWorkspace* poutWorkspace)
	{
		FeatureClass* poutFeatureClass = NULL;
		poutFeatureClass = poutWorkspace->OpenFeatureClass(className);
		if(poutFeatureClass!=NULL)
		{
			return NULL;
		}

		GField * pField  = NULL;
		GField_2* pField_2  = NULL;
		GFields* pFields = NULL;
		FieldFactory* pFieldFactory = augeGetFieldFactoryInstance();
		pFields = pFieldFactory->CreateFields();
		pField = pFieldFactory->CreateField();
		pField_2 = pField->Field_2();
		pField_2->SetName(AUGE_DEFAULT_GEOM_FIELD);
		pField_2->SetType(augeFieldTypeGeometry);
		GeometryDef		*pGeometryDef = pField->GetGeometryDef();
		GeometryDef_2	*pGeometryDef_2=pGeometryDef->GetGeometryDef_2();
		pGeometryDef_2->SetSRID(srid);
		pGeometryDef_2->SetGeometryType(augeGTPoint);
		pGeometryDef_2->SetDimension(2);
		pFields->Add(pField);

		poutFeatureClass = poutWorkspace->CreateFeatureClass(className, pFields);
		pFields->Release();
		return poutFeatureClass;
	}

	RESULTCODE GridPointGeneratorImpl::Generate(FeatureClass* poutFeatureClass)
	{
		g_uint rows, cols;
		rows = ceil(m_extent.GetHeight()/ m_cell_size);
		cols = ceil(m_extent.GetWidth() / m_cell_size);

		double cx,cy;
		cx = (m_extent.m_xmin + m_extent.m_xmax) / 2.0;
		cy = (m_extent.m_ymin + m_extent.m_ymax) / 2.0;

		double width = cols * m_cell_size;
		double height= rows * m_cell_size;

		WKBPoint point;
		point.byteOrder = coDefaultByteOrder;
		point.wkbType = wkbPoint;

		Geometry* pGeometry = NULL;
		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();

		GValue* pValue = NULL;
		Feature* pnFeature = poutFeatureClass->NewFeature();
		FeatureInsertCommand* cmd = poutFeatureClass->CreateInsertCommand();

		double xmin = cx - width / 2.0;
		double ymin = cy - height/ 2.0;
		double x, y = ymin;
		for(g_uint i=0; i<=rows; i++)
		{
			x = xmin;
			for(g_uint j=0; j<=cols; j++)
			{
				point.point.x = x;
				point.point.y = y;

				pGeometry = pGeometryFactory->CreateGeometryFromWKB((g_byte*)(&point));
				if(pGeometry!=NULL)
				{
					pValue = new GValue(pGeometry);
					pnFeature->SetValue(AUGE_DEFAULT_GEOM_FIELD, pValue);
					cmd->Insert(pnFeature);
				}
				x+=m_cell_size;
			}
			y+=m_cell_size;
		}
		pnFeature->Release();
		cmd->Release();

		return AG_SUCCESS;
	}

	void GridPointGeneratorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}
}
