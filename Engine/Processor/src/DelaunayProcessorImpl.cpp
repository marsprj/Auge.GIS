#include "DelaunayProcessorImpl.h"

#include "AugeData.h"
#include "AugeCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeUser.h"

#include "Delaunay.h"

#include <fstream>

namespace auge
{
	DelaunayProcessorImpl::DelaunayProcessorImpl()
	{
		m_user = 0;
		m_in_z_field.clear();
	}

	DelaunayProcessorImpl::~DelaunayProcessorImpl()
	{
	}

	void DelaunayProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void DelaunayProcessorImpl::SetInputFeatureClass(const char* className)
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

	void DelaunayProcessorImpl::SetInputZField(const char* fname)
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

	void DelaunayProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void DelaunayProcessorImpl::SetOutputFeatureClass(const char* className)
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

	void DelaunayProcessorImpl::SetOutputZField(const char* fname)
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

	const char* DelaunayProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char* DelaunayProcessorImpl::GetInputFatureClass()
	{
		return m_in_class_name.empty() ? NULL : m_in_class_name.c_str();
	}

	const char* DelaunayProcessorImpl::GetInputZField()
	{
		return m_in_z_field.empty() ? NULL : m_in_z_field.c_str();
	}

	const char*	DelaunayProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	DelaunayProcessorImpl::GetOutputFatureClass()
	{
		return m_out_class_name.empty() ? NULL : m_out_class_name.c_str();
	}
	
	RESULTCODE DelaunayProcessorImpl::Execute()
	{
		GError	*pError  = augeGetErrorInstance();
		GLogger	*pLogger = augeGetLoggerInstance();

		const char* sourceName_in = GetInputDataSource();
		const char* className_in  = GetInputFatureClass();
		const char* z_field_in = GetInputZField();
		
		const char* sourceName_out= GetOutputDataSource();
		const char* className_out = GetOutputFatureClass();
		const char* z_field_out = GetInputZField();

		g_uint srid = 0;

		if(sourceName_in==NULL)
		{
			return AG_FAILURE;
		}
		if(className_in==NULL)
		{
			return AG_FAILURE;
		}
		//if(z_field_in==NULL)
		//{
		//	return AG_FAILURE;
		//}

		if(sourceName_out==NULL)
		{
			return AG_FAILURE;
		}
		if(className_out==NULL)
		{
			return AG_FAILURE;
		}
		//if(z_field_out==NULL)
		//{
		//	return AG_FAILURE;
		//}
		
		FeatureClass		*pinFeatureClass = NULL;
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

		srid = pinFeatureClass->GetSRID();
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
	
		//pField = pinFeatureClass->GetField(z_field_in);
		//if(pField==NULL)
		//{
		//	pinFeatureClass->Release();
		//	poutWorkspace->Release();
		//	return AG_FAILURE;
		//}
		//augeFieldType ftype = pField->GetType();
		//if((ftype!=augeFieldTypeInt) && (ftype!=augeFieldTypeShort) && (ftype!=augeFieldTypeFloat) && (ftype!=augeFieldTypeDouble))
		//{
		//	pinFeatureClass->Release();
		//	poutWorkspace->Release();
		//	return AG_FAILURE;
		//}

		g_uint vertex_count = 0;
		delaunay_vertext_t* vertexes = NULL;
		if(!LoadVertex(pinFeatureClass, &vertexes, vertex_count))
		{
			pinFeatureClass->Release();
			poutWorkspace->Release();
			return AG_FAILURE;
		}

		Delaunay delaunay;
		delaunay.SetVertex(vertexes, vertex_count);
		delaunay.Triangulate();

		WriteTriangles(delaunay, className_out, poutWorkspace, srid);
		WriteEdges(delaunay, className_out, poutWorkspace, srid);

		pinFeatureClass->Release();
		poutWorkspace->Release();

		return AG_SUCCESS;
	}

	void DelaunayProcessorImpl::Release()
	{
		delete this;
	}

	void DelaunayProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}


	FeatureClass* DelaunayProcessorImpl::CreateTriangleFeatureClass(const char* outClassName, FeatureWorkspace* poutWorkspace, g_uint srid)
	{
		char triClassName[AUGE_NAME_MAX];
		g_sprintf(triClassName, "%s_triangle", outClassName);

		GField  *pField = NULL;
		GFields *pFields = NULL;
		FieldFactory* pFieldFactory = augeGetFieldFactoryInstance();

		GField_2  *pField_2 = NULL;
		pField = pFieldFactory->CreateField();
		pField_2 = pField->Field_2();
		pField_2->SetName(AUGE_DEFAULT_GEOM_FIELD);
		pField_2->SetType(augeFieldTypeGeometry);

		GeometryDef* pGeometryDef = NULL;
		GeometryDef_2 *pGeometryDef_2 = NULL;
		pGeometryDef = pField->GetGeometryDef();
		pGeometryDef_2 = pGeometryDef->GetGeometryDef_2();
		pGeometryDef_2->SetGeometryType(augeGTPolygon);
		pGeometryDef_2->SetSRID(srid);
		pFields = pFieldFactory->CreateFields();
		pFields->Add(pField);

		FeatureClass* ptriFeatureCass = NULL;
		ptriFeatureCass = poutWorkspace->CreateFeatureClass(triClassName, pFields);
		pFields->Release();

		return ptriFeatureCass;
	}

	bool DelaunayProcessorImpl::WriteTriangles(Delaunay& delaunay, const char* outClassName, FeatureWorkspace* poutWorkspace, g_uint srid)
	{
		FeatureClass* ptriFeatureClass = NULL;
		ptriFeatureClass = CreateTriangleFeatureClass(outClassName, poutWorkspace, srid);
		if(ptriFeatureClass==NULL)
		{
			return false;
		}

		g_uint count = delaunay.GetTriangleCount();
		delaunay_triangle_t* triangles = delaunay.GetTriangle();
		delaunay_vertext_t*  vertexes = delaunay.GetVertex();

		size_t wkbSize = sizeof(WKBPolygon) + sizeof(auge::Point) * 3;
		WKBPolygon* pWKBPolygon = NULL;
		pWKBPolygon = (WKBPolygon*)malloc(wkbSize);
		memset(pWKBPolygon, 0, wkbSize);
		pWKBPolygon->byteOrder = coDefaultByteOrder;
		pWKBPolygon->wkbType = wkbPolygon;
		pWKBPolygon->numRings = 1;
		pWKBPolygon->rings[0].numPoints = 4;
		auge::Point* ptr = &(pWKBPolygon->rings[0].points[0]);

		Geometry* pGeometry = NULL;
		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();

		GValue* pGeoValue = NULL;
		Feature* pFeature = ptriFeatureClass->NewFeature();
		FeatureInsertCommand* cmd = ptriFeatureClass->CreateInsertCommand();

		g_uint v1, v2, v3;
		for(g_uint i=0; i<count; i++)
		//for(g_uint i=0; i<6; i++)
		{
			v1 = triangles[i].v1;
			v2 = triangles[i].v2;
			v3 = triangles[i].v3;

			ptr[0].x = vertexes[v1].x;
			ptr[0].y = vertexes[v1].y;
			ptr[1].x = vertexes[v2].x;
			ptr[1].y = vertexes[v2].y;
			ptr[2].x = vertexes[v3].x;
			ptr[2].y = vertexes[v3].y;
			ptr[3].x = vertexes[v1].x;
			ptr[3].y = vertexes[v1].y;

			pGeometry = pGeometryFactory->CreateGeometryFromWKB((g_byte*)pWKBPolygon, true);
			pGeoValue = new GValue(pGeometry);
			pFeature->SetValue(AUGE_DEFAULT_GEOM_FIELD, pGeoValue);

			cmd->Insert(pFeature);
		}
		pFeature->Release();
		cmd->Release();
		ptriFeatureClass->Refresh();
		ptriFeatureClass->Release();

		free(pWKBPolygon);

		return true;
	}

	FeatureClass* DelaunayProcessorImpl::CreateEdgeFeatureClass(const char* outClassName, FeatureWorkspace* poutWorkspace, g_uint srid)
	{
		char triClassName[AUGE_NAME_MAX];
		g_sprintf(triClassName, "%s_edge", outClassName);

		GField  *pField = NULL;
		GFields *pFields = NULL;
		FieldFactory* pFieldFactory = augeGetFieldFactoryInstance();

		GField_2  *pField_2 = NULL;
		pField = pFieldFactory->CreateField();
		pField_2 = pField->Field_2();
		pField_2->SetName(AUGE_DEFAULT_GEOM_FIELD);
		pField_2->SetType(augeFieldTypeGeometry);

		GeometryDef* pGeometryDef = NULL;
		GeometryDef_2 *pGeometryDef_2 = NULL;
		pGeometryDef = pField->GetGeometryDef();
		pGeometryDef_2 = pGeometryDef->GetGeometryDef_2();
		pGeometryDef_2->SetGeometryType(augeGTLineString);
		pGeometryDef_2->SetSRID(srid);
		pFields = pFieldFactory->CreateFields();
		pFields->Add(pField);

		FeatureClass* ptriFeatureCass = NULL;
		ptriFeatureCass = poutWorkspace->CreateFeatureClass(triClassName, pFields);
		pFields->Release();

		return ptriFeatureCass;
	}

	bool DelaunayProcessorImpl::WriteEdges(Delaunay& delaunay, const char* outClassName, FeatureWorkspace* poutWorkspace, g_uint srid)
	{
		FeatureClass* ptriFeatureClass = NULL;
		ptriFeatureClass = CreateEdgeFeatureClass(outClassName, poutWorkspace, srid);
		if(ptriFeatureClass==NULL)
		{
			return false;
		}

		g_uint count = delaunay.GetEdgeCount();
		delaunay_edge_t* edges = delaunay.GetEdge();
		delaunay_vertext_t*  vertexes = delaunay.GetVertex();

		size_t wkbSize = sizeof(WKBLineString) + sizeof(auge::Point);
		WKBLineString* pWKBLineString = NULL;
		pWKBLineString = (WKBLineString*)malloc(wkbSize);
		memset(pWKBLineString, 0, wkbSize);
		pWKBLineString->byteOrder = coDefaultByteOrder;
		pWKBLineString->wkbType = wkbLineString;
		pWKBLineString->numPoints = 2;
		auge::Point* ptr = &(pWKBLineString->points[0]);

		Geometry* pGeometry = NULL;
		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();

		GValue* pGeoValue = NULL;
		Feature* pFeature = ptriFeatureClass->NewFeature();
		FeatureInsertCommand* cmd = ptriFeatureClass->CreateInsertCommand();

		g_int v1, v2;
		for(g_uint i=0; i<count; i++)
		{
			v1 = edges[i].v1;
			v2 = edges[i].v2;

			ptr[0].x = vertexes[v1].x;
			ptr[0].y = vertexes[v1].y;
			ptr[1].x = vertexes[v2].x;
			ptr[1].y = vertexes[v2].y;

			pGeometry = pGeometryFactory->CreateGeometryFromWKB((g_byte*)pWKBLineString, true);
			pGeoValue = new GValue(pGeometry);
			pFeature->SetValue(AUGE_DEFAULT_GEOM_FIELD, pGeoValue);

			cmd->Insert(pFeature);
		}
		pFeature->Release();
		cmd->Release();
		ptriFeatureClass->Refresh();
		ptriFeatureClass->Release();

		free(pWKBLineString);

		return true;
	}

	bool DelaunayProcessorImpl::LoadVertex(FeatureClass* pinFeatureClass, delaunay_vertext_t** vertexes, g_uint& vertex_count)
	{
		double zvalue = 0.0f;
		GField* pField = NULL;
		Geometry* pGeometry = NULL;
		Feature* pFeature = NULL;
		FeatureCursor* pCursor = NULL;
		GQuery* pQuery = NULL;
		FilterFactory* pFilterFactory = augeGetFilterFactoryInstance();
		augeFieldType ftype = augeFieldTypeNone;

		vertex_count = pinFeatureClass->GetCount();
		if(vertex_count==0)
		{
			return false;
		}

		*vertexes = (delaunay_vertext_t*)malloc((vertex_count+3) * sizeof(delaunay_vertext_t));		
		//delaunay_vertext_t* ptsss = (delaunay_vertext_t*)malloc((vertex_count+3) * sizeof(delaunay_vertext_t));	
		memset(*vertexes, 0, (vertex_count+3) * sizeof(delaunay_vertext_t));
		delaunay_vertext_t* ptr = *vertexes;

		pField = pinFeatureClass->GetField(m_in_z_field.c_str());
		if(pField!=NULL)
		{
			ftype = pField->GetType();
		}

		vertex_count = 0;
		pQuery = pFilterFactory->CreateQuery();
		if(pField!=NULL)
		{
			pQuery->AddSubField(m_in_z_field.c_str());
		}
		pQuery->AddSubField(pinFeatureClass->GetFields()->GetGeometryField()->GetName());
		pCursor = pinFeatureClass->Query(pQuery);
		while((pFeature=pCursor->NextFeature())!=NULL)
		{
			pGeometry = pFeature->GetGeometry();
			if(pGeometry!=NULL)
			{
				if(pField!=NULL)
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
				}

				WKBPoint* pWKBPoint = (WKBPoint*)pGeometry->AsBinary();
				ptr->x = pWKBPoint->point.x;
				ptr->y = pWKBPoint->point.y;
				ptr->id = pFeature->GetFID();

				ptr++;
				vertex_count++;
			}

			pFeature->Release();
		}
		pCursor->Release();
		pQuery->Release();

		return (vertex_count>0);
	}
}
