#include "FeatureIDWProcessorImpl.h"
#include "Kmean.h"

#include "AugeData.h"
#include "AugeCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeUser.h"

#include <fstream>

namespace auge
{
	FeatureIDWProcessorImpl::FeatureIDWProcessorImpl()
	{
		m_user = 0;
		m_cell_size = 0.0f;
		m_in_z_field.clear();

		m_rows = 0;
		m_cols = 0;
		m_point_count = 0;
		m_source_points = NULL;
	}

	FeatureIDWProcessorImpl::~FeatureIDWProcessorImpl()
	{
		if(m_source_points!=NULL)
		{
			free(m_source_points);
			m_source_points = NULL;
		}
	}

	void FeatureIDWProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void FeatureIDWProcessorImpl::SetInputFeatureClass(const char* className)
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

	void FeatureIDWProcessorImpl::SetInputZField(const char* fname)
	{
		if(fname==NULL)
		{
			m_in_z_field.clear();
		}
		else
		{
			m_in_z_field = fname;
		}
	}

	void FeatureIDWProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void FeatureIDWProcessorImpl::SetOutputFeatureClass(const char* className)
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

	void FeatureIDWProcessorImpl::SetOutputZField(const char* fname)
	{
		if(fname==NULL)
		{
			m_out_z_field.clear();
		}
		else
		{
			m_out_z_field = fname;
		}
	}

	void FeatureIDWProcessorImpl::SetExtent(GEnvelope& extent)
	{
		m_extent = extent;
	}

	void FeatureIDWProcessorImpl::SetCellSize(double cellSize)
	{
		m_cell_size = cellSize;
	}

	const char* FeatureIDWProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char* FeatureIDWProcessorImpl::GetInputFatureClass()
	{
		return m_in_class_name.empty() ? NULL : m_in_class_name.c_str();
	}

	const char* FeatureIDWProcessorImpl::GetInputZField()
	{
		return m_in_z_field.empty() ? NULL : m_in_z_field.c_str();
	}

	const char*	FeatureIDWProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	FeatureIDWProcessorImpl::GetOutputFatureClass()
	{
		return m_out_source_name.empty() ? NULL : m_out_class_name.c_str();
	}
	
	double FeatureIDWProcessorImpl::GetCellSize()
	{
		return m_cell_size;
	}

	GEnvelope& FeatureIDWProcessorImpl::GetExtent()
	{
		return m_extent;
	}

	RESULTCODE FeatureIDWProcessorImpl::Execute()
	{
		GError	*pError  = augeGetErrorInstance();
		GLogger	*pLogger = augeGetLoggerInstance();

		const char* sourceName_in = GetInputDataSource();
		const char* className_in  = GetInputFatureClass();
		const char* z_field_in = GetInputZField();
		
		const char* sourceName_out= GetOutputDataSource();
		const char* className_out = GetOutputFatureClass();
		const char* z_field_out = GetInputZField();

		if(sourceName_in==NULL)
		{
			return AG_FAILURE;
		}
		if(className_in==NULL)
		{
			return AG_FAILURE;
		}
		if(z_field_in==NULL)
		{
			return AG_FAILURE;
		}

		if(sourceName_out==NULL)
		{
			return AG_FAILURE;
		}
		if(className_out==NULL)
		{
			return AG_FAILURE;
		}
		if(z_field_out==NULL)
		{
			return AG_FAILURE;
		}

		if(m_cell_size<=0)
		{
			return AG_FAILURE;
		}
		
		if(!m_extent.IsValid())
		{
			return AG_FAILURE;
		}

		FeatureClass		*pinFeatureClass = NULL;
		FeatureClass		*poutFeatureClass= NULL;
		FeatureWorkspace	*pinWorkspace = NULL;
		FeatureWorkspace	*poutWorkspace= NULL;
		ConnectionManager	*pConnManager = augeGetConnectionManagerInstance();

		pinWorkspace = dynamic_cast<FeatureWorkspace*>(pConnManager->GetWorkspace(m_user, sourceName_in));
		if(pinWorkspace==NULL)
		{
			return AG_FAILURE;
		}

		poutWorkspace = dynamic_cast<FeatureWorkspace*>(pConnManager->NewWorkspace(m_user, sourceName_out));
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
	
		pField = pinFeatureClass->GetField(z_field_in);
		if(pField==NULL)
		{
			pinFeatureClass->Release();
			poutWorkspace->Release();
			return AG_FAILURE;
		}
		augeFieldType ftype = pField->GetType();
		if((ftype!=augeFieldTypeInt) && (ftype!=augeFieldTypeShort) && (ftype!=augeFieldTypeFloat) && (ftype!=augeFieldTypeDouble))
		{
			pinFeatureClass->Release();
			poutWorkspace->Release();
			return AG_FAILURE;
		}

		if(!ReadSourcePoints(pinFeatureClass))
		{
			pinFeatureClass->Release();
			poutWorkspace->Release();
			return AG_FAILURE;
		}

		g_uint srid = pinFeatureClass->GetSRID();
		poutFeatureClass = CreateOutputFeatureClass(className_out, z_field_out, srid, poutWorkspace);
		if(poutFeatureClass==NULL)
		{
			pinFeatureClass->Release();
			poutWorkspace->Release();
			return AG_FAILURE;
		}

		Interpolate(poutFeatureClass);

		pinFeatureClass->Release();
		poutFeatureClass->Release();
		poutWorkspace->Release();

		return AG_SUCCESS;
	}

	void FeatureIDWProcessorImpl::Release()
	{
		delete this;
	}

	void FeatureIDWProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}

	bool FeatureIDWProcessorImpl::ReadSourcePoints(FeatureClass* pinFeatureClass)
	{
		double zvalue = 0.0f;
		GField* pField = NULL;
		Geometry* pGeometry = NULL;
		Feature* pFeature = NULL;
		FeatureCursor* pCursor = NULL;
		GQuery* pQuery = NULL;
		FilterFactory* pFilterFactory = augeGetFilterFactoryInstance();
		augeFieldType ftype = augeFieldTypeNone;

		if(m_source_points!=NULL)
		{
			free(m_source_points);
			m_source_points = NULL;
		}
		m_point_count = pinFeatureClass->GetCount();
		m_source_points = (idw_source_point_t*)malloc(m_point_count * sizeof(idw_source_point_t));
		if(m_source_points==NULL)
		{
			return false;
		}
		memset(m_source_points, 0, m_point_count * sizeof(idw_source_point_t));
		idw_source_point_t* ptr = m_source_points;
		
		pField = pinFeatureClass->GetField(m_in_z_field.c_str());
		ftype = pField->GetType();

		m_point_count = 0;

		pQuery = pFilterFactory->CreateQuery();
		pQuery->AddSubField(m_in_z_field.c_str());
		pQuery->AddSubField(pinFeatureClass->GetFields()->GetGeometryField()->GetName());
		pCursor = pinFeatureClass->Query(pQuery);
		while((pFeature=pCursor->NextFeature())!=NULL)
		{
			pGeometry = pFeature->GetGeometry();
			if(pGeometry!=NULL)
			{
				switch(ftype)
				{
				case augeFieldTypeShort:
					ptr->z = pFeature->GetShort(m_in_z_field.c_str());
					break;
				case augeFieldTypeInt:
					ptr->z = pFeature->GetInt(m_in_z_field.c_str());
					break;
				case augeFieldTypeFloat:
					ptr->z = pFeature->GetFloat(m_in_z_field.c_str());
					break;
				case augeFieldTypeDouble:
					ptr->z = pFeature->GetDouble(m_in_z_field.c_str());
					break;
				}
				WKBPoint* pWKBPoint = (WKBPoint*)pGeometry->AsBinary();
				ptr->x = pWKBPoint->point.x;
				ptr->y = pWKBPoint->point.y;

				ptr++;
				m_point_count++;
			}

			pFeature->Release();
		}
		pCursor->Release();
		pQuery->Release();

		return (m_point_count>0);
	}

	FeatureClass* FeatureIDWProcessorImpl::CreateOutputFeatureClass(const char* className, const char* zfield, g_uint srid, FeatureWorkspace* poutWorkspace)
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
		pField_2->SetName(zfield);
		pField_2->SetType(augeFieldTypeDouble);
		pFields->Add(pField);

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

	bool FeatureIDWProcessorImpl::Interpolate(FeatureClass* poutFeatureClass)
	{
		m_rows = ceil(m_extent.GetHeight()/ m_cell_size);
		m_cols = ceil(m_extent.GetWidth() / m_cell_size);

		double cx,cy;
		cx = (m_extent.m_xmin + m_extent.m_xmax) / 2.0;
		cy = (m_extent.m_ymin + m_extent.m_ymax) / 2.0;

		double width = m_cols * m_cell_size;
		double height= m_rows * m_cell_size;

		WKBPoint point;
		point.byteOrder = coDefaultByteOrder;
		point.wkbType = wkbPoint;

		Geometry* pGeometry = NULL;
		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();

		GValue* pZValue = NULL;
		GValue* pGeoValue = NULL;
		Feature* pnFeature = poutFeatureClass->NewFeature();
		FeatureInsertCommand* cmd = poutFeatureClass->CreateInsertCommand();

		double xmin = cx - width / 2.0;
		double ymin = cy - height/ 2.0;
		double x, y = ymin;
		double zvalue = 0.0f;
		for(g_uint i=0; i<=m_rows; i++)
		{
			x = xmin;
			for(g_uint j=0; j<=m_cols; j++)
			{
				point.point.x = x;
				point.point.y = y;

				zvalue = IDW_Interplate(x, y);
				//zvalue = IDW_Interplate(82.943, 46.864);

				pGeometry = pGeometryFactory->CreateGeometryFromWKB((g_byte*)(&point));
				if(pGeometry!=NULL)
				{
					pZValue = new GValue(zvalue);
					pnFeature->SetValue(m_out_z_field.c_str(), pZValue);
					pGeoValue = new GValue(pGeometry);
					pnFeature->SetValue(AUGE_DEFAULT_GEOM_FIELD, pGeoValue);
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

	double FeatureIDWProcessorImpl::IDW_Interplate(double x, double y)
	{
		double zvalue = 0.0f;
		double v = 0.0f;
		double w = 0.0f;
		double sum_w = 0.0f;

		//std::ofstream os("g:\\temp\\out.txt");

		idw_source_point_t *ptr = m_source_points;
		for(g_uint i=0; i<m_point_count; i++, ptr++)
		{
			w = 1.0 / (pow(ptr->x-x, 2.0)+pow(ptr->y-y, 2.0));
			v = w * ptr->z;
			zvalue += v;
			sum_w += w;

			//os<<ptr->z<<"\t"<<w<<"\t"<<ptr->x<<"\t"<<ptr->y<<std::endl;
		}

		//os.flush();
		//os.close();
		return zvalue / sum_w;
	}
}
