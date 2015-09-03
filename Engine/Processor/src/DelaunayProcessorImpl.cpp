#include "DelaunayProcessorImpl.h"
#include "Kmean.h"

#include "AugeData.h"
#include "AugeCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeUser.h"

#include <fstream>

namespace auge
{
	DelaunayProcessorImpl::DelaunayProcessorImpl()
	{
		m_user = 0;
		m_vertex_count = 0;
		m_vertexes = NULL;
		m_triangle_count = 0;
		m_triangles = NULL;
		m_edge_count = 0;
		m_edges = NULL;
		
		m_in_z_field.clear();
	}

	DelaunayProcessorImpl::~DelaunayProcessorImpl()
	{
		if(m_vertexes!=NULL)
		{
			free(m_vertexes);
			m_vertexes = NULL;
		}
		if(m_triangles!=NULL)
		{
			free(m_triangles);
			m_triangles = NULL;
		}
		if(m_edges!=NULL)
		{
			free(m_edges);
			m_edges = NULL;
		}
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
		return m_out_source_name.empty() ? NULL : m_out_class_name.c_str();
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

		if(!LoadVertex(pinFeatureClass))
		{
			pinFeatureClass->Release();
			poutWorkspace->Release();
			return AG_FAILURE;
		}

		Triangulate();

		//WriteTriangles(className_out, poutWorkspace, srid);
		WriteEdges(className_out, poutWorkspace, srid);

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

	bool DelaunayProcessorImpl::LoadVertex(FeatureClass* pinFeatureClass)
	{
		double zvalue = 0.0f;
		GField* pField = NULL;
		Geometry* pGeometry = NULL;
		Feature* pFeature = NULL;
		FeatureCursor* pCursor = NULL;
		GQuery* pQuery = NULL;
		FilterFactory* pFilterFactory = augeGetFilterFactoryInstance();
		augeFieldType ftype = augeFieldTypeNone;

		if(m_vertexes!=NULL)
		{
			free(m_vertexes);
			m_vertexes = NULL;
		}
		m_vertex_count = pinFeatureClass->GetCount();
		m_vertexes = (d_vertext_t*)malloc((m_vertex_count+3) * sizeof(d_vertext_t));
		if(m_vertexes==NULL)
		{
			return false;
		}
		memset(m_vertexes, 0, (m_vertex_count+3) * sizeof(d_vertext_t));
		d_vertext_t* ptr = m_vertexes;
		
		pField = pinFeatureClass->GetField(m_in_z_field.c_str());
		if(pField!=NULL)
		{
			ftype = pField->GetType();
		}

		m_vertex_count = 0;
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

				ptr++;
				m_vertex_count++;
			}

			pFeature->Release();
		}
		pCursor->Release();
		pQuery->Release();

		return (m_vertex_count>0);
	}

	bool DelaunayProcessorImpl::Triangulate()
	{
		g_int		*complete = NULL;
		g_int		trimax, emax =200;
		g_int		nedge = 0;
		g_int		status = 0;
		
		bool		inside;
		g_int		i,j,k;
		double		xp,yp,x1,y1,x2,y2,x3,y3,xc,yc,r;
		double		xmin,xmax,ymin,ymax,xmid,ymid;
		double		dx,dy,dmax;

		/* Allocate memory for the completeness list, flag for each triangle */
		trimax = m_vertex_count << 2;
		complete = (g_int *)malloc(trimax * sizeof(g_int));
		if(complete==NULL)
		{
			status = 1;
			return false;
		}
		memset(complete, 0, trimax * sizeof(g_int));

		/* Allocate memory for the edge list */
		m_edges = (d_edge_t *)malloc(emax*sizeof(d_edge_t));
		if(m_edges==NULL)
		{
			free(complete);	complete=NULL;
			status = 2;
			return false;
		}
		memset(m_edges, 0, emax*sizeof(d_edge_t));

		/* Allocate memory for the triangle list */
		m_triangles = (d_triangle_t *)malloc(trimax*sizeof(d_triangle_t));
		if(m_triangles==NULL)
		{
			free(complete);	complete=NULL;
			status = 2;
			return false;
		}
		memset(m_triangles, 0, trimax*sizeof(d_triangle_t));

		// compute the extent of input vertexes
		ComputeExtent(xmin,ymin,xmax,ymax);

		dx = xmax - xmin;
		dy = ymax - ymin;
		dmax = (dx > dy) ? dx : dy;
		xmid = (xmax + xmin) / 2.0;
		ymid = (ymax + ymin) / 2.0;
		GenerateSuperTriangle(xmid, ymid, dmax);
		complete[0] = AUGE_FALSE;

		for(i=0; i<m_vertex_count; i++)
		//for(i=0; i<1; i++)
		{
			xp = m_vertexes[i].x;
			yp = m_vertexes[i].y;
			nedge = 0; 

			memset(complete, 0, trimax * sizeof(g_int));

			/*
				Set up the edge buffer.
				If the point (xp,yp) lies inside the circumcircle then the
				three edges of that triangle are added to the edge buffer
				and that triangle is removed.
		   */
			for(j=0; j<m_triangle_count; j++)
			{
				if(complete[j]==AUGE_TRUE)
				{
					continue;
				}

				x1 = m_vertexes[m_triangles[j].v1].x;
				y1 = m_vertexes[m_triangles[j].v1].y;
				x2 = m_vertexes[m_triangles[j].v2].x;
				y2 = m_vertexes[m_triangles[j].v2].y;
				x3 = m_vertexes[m_triangles[j].v3].x;
				y3 = m_vertexes[m_triangles[j].v3].y;

				inside = CircumCircle(xp,yp,x1,y1,x2,y2,x3,y3,&xc,&yc,&r);
				if (xc < xp && ((xp-xc)*(xp-xc)) > r)	//???
				{
					complete[j] = AUGE_TRUE;
				}

				if(inside)
				{
					if(nedge+3 > emax)
					{
						emax += 100;
						m_edges = (d_edge_t*)realloc(m_edges, emax * sizeof(d_edge_t));
						if(m_edges==NULL)
						{
							free(complete);	complete=NULL;
							status = 3;
							return false;
						}
					}

					m_edges[nedge+0].v1 = m_triangles[j].v1;
					m_edges[nedge+0].v2 = m_triangles[j].v2;
					m_edges[nedge+1].v1 = m_triangles[j].v2;
					m_edges[nedge+1].v2 = m_triangles[j].v3;
					m_edges[nedge+2].v1 = m_triangles[j].v3;
					m_edges[nedge+2].v2 = m_triangles[j].v1;
					nedge += 3;
					m_triangles[j] = m_triangles[m_triangle_count-1];
					complete[j] = complete[m_triangle_count-1];
					m_triangle_count--;
					j--;
				}
			}

			/*
				Tag multiple edges
				Note: if all triangles are specified anticlockwise then all
				interior edges are opposite pointing in direction.
			*/
			for(j=0; j<nedge; j++)
			{
				for(k=j+1; k<nedge; k++)
				{
					if((m_edges[j].v1==m_edges[k].v2) && (m_edges[j].v2==m_edges[k].v1))
					{
						m_edges[j].v1 = -1;
						m_edges[j].v2 = -1;
						m_edges[k].v1 = -1;
						m_edges[k].v2 = -1;
					}

					/* Shouldn't need the following, see note above */
					if ((m_edges[j].v1 == m_edges[k].v1) && (m_edges[j].v2 == m_edges[k].v2)) 
					{
						m_edges[j].v1 = -1;
						m_edges[j].v2 = -1;
						m_edges[k].v1 = -1;
						m_edges[k].v2 = -1;
					}
				}
			}

			/*
				Form new triangles for the current point
				Skipping over any tagged edges.
				All edges are arranged in clockwise order.
			*/
			for(j=0; j<nedge; j++)
			{
				if(m_edges[j].v1<0 || m_edges[j].v2<0)
				{
					continue;
				}
				if(m_triangle_count>trimax)
				{
					free(complete);	complete=NULL;
					status = 4;
					return false;
				}
				m_triangles[m_triangle_count].v1 = m_edges[j].v1;
				m_triangles[m_triangle_count].v2 = m_edges[j].v2;
				m_triangles[m_triangle_count].v3 = i;
				complete[m_triangle_count] = AUGE_FALSE;
				m_triangle_count++;
			}			
		}

		/*
			Remove triangles with supertriangle vertices
			These are triangles which have a vertex number greater than nv
		*/
		d_triangle_t* ptri = NULL;
		for(i=0; i<m_triangle_count; i++)
		{
			ptri = m_triangles+i;
			if(m_triangles[i].v1>=m_vertex_count || m_triangles[i].v2>=m_vertex_count || m_triangles[i].v2>=m_vertex_count)
			{
				m_triangles[i] = m_triangles[m_triangle_count-1];
				m_triangle_count--;
				i--;
			}
		}

		// Generate Edge List
		g_uint count = 0;
		count = m_triangle_count * 3;
		m_edges = (d_edge_t*)malloc(count*sizeof(d_edge_t));
		memset(m_edges, 0, count*sizeof(d_edge_t));

		ptri = m_triangles;
		for(i=0; i<m_triangle_count; i++, ptri++)
		{
			//if(m_edge_count>=count)
			//{
			//	count += m_triangle_count;
			//	m_edges = (d_edge_t*)realloc(m_edges, count*sizeof(d_edge_t));
			//}
			if(!FindEdge(ptri->v1, ptri->v2))
			{
				m_edges[m_edge_count].v1 = ptri->v1;
				m_edges[m_edge_count].v2 = ptri->v2;
				m_edge_count++;
			}
			if(!FindEdge(ptri->v2, ptri->v3))
			{
				m_edges[m_edge_count].v1 = ptri->v2;
				m_edges[m_edge_count].v2 = ptri->v3;
				m_edge_count++;
			}
			if(!FindEdge(ptri->v3, ptri->v1))
			{
				m_edges[m_edge_count].v1 = ptri->v3;
				m_edges[m_edge_count].v2 = ptri->v1;
				m_edge_count++;
			}
		}

		return true;
	}

	void DelaunayProcessorImpl::ComputeExtent(double& xmin, double& ymin, double& xmax, double& ymax)
	{
		xmin = m_vertexes[0].x;
		xmax = m_vertexes[0].x;
		ymin = m_vertexes[0].y;
		ymax = m_vertexes[0].y;

		for(g_uint i=1; i<m_vertex_count; i++)
		{
			if(xmin > m_vertexes[i].x)	xmin = m_vertexes[i].x;
			if(xmax < m_vertexes[i].x)	xmax = m_vertexes[i].x;
			if(ymin > m_vertexes[i].y)	ymin = m_vertexes[i].y;
			if(ymax < m_vertexes[i].y)	ymax = m_vertexes[i].y;
		}
	}

	void DelaunayProcessorImpl::GenerateSuperTriangle(double cx, double cy, double size)
	{
		m_vertexes[m_vertex_count + 0].x = cx - 20 * size;
		m_vertexes[m_vertex_count + 0].y = cy - size;
		m_vertexes[m_vertex_count + 0].z = 0.0;

		m_vertexes[m_vertex_count + 1].x = cx;
		m_vertexes[m_vertex_count + 1].y = cy + 20 * size;
		m_vertexes[m_vertex_count + 1].z = 0.0;

		m_vertexes[m_vertex_count + 2].x = cx + 20 * size;
		m_vertexes[m_vertex_count + 2].y = cy - size;
		m_vertexes[m_vertex_count + 2].z = 0.0;

		m_triangles[0].v1 = m_vertex_count + 0;
		m_triangles[0].v2 = m_vertex_count + 1;
		m_triangles[0].v3 = m_vertex_count + 2;

		m_triangle_count = 1;
	}

	bool DelaunayProcessorImpl::CircumCircle(double xp,double yp, double x1,double y1,double x2,double y2,double x3,double y3, double *xc,double *yc,double *rsqr)
	{
		double m1,m2,mx1,mx2,my1,my2;
		double dx,dy,drsqr;
		double fabsy1y2 = fabs(y1-y2);
		double fabsy2y3 = fabs(y2-y3);

		/* Check for coincident points */
		if (fabsy1y2 < AUGE_EPSILON && fabsy2y3 < AUGE_EPSILON)
			return(FALSE);

		if (fabsy1y2 < AUGE_EPSILON) {
			m2 = - (x3-x2) / (y3-y2);
			mx2 = (x2 + x3) / 2.0;
			my2 = (y2 + y3) / 2.0;
			*xc = (x2 + x1) / 2.0;
			*yc = m2 * (*xc - mx2) + my2;
		} else if (fabsy2y3 < AUGE_EPSILON) {
			m1 = - (x2-x1) / (y2-y1);
			mx1 = (x1 + x2) / 2.0;
			my1 = (y1 + y2) / 2.0;
			*xc = (x3 + x2) / 2.0;
			*yc = m1 * (*xc - mx1) + my1;
		} else {
			m1 = - (x2-x1) / (y2-y1);
			m2 = - (x3-x2) / (y3-y2);
			mx1 = (x1 + x2) / 2.0;
			mx2 = (x2 + x3) / 2.0;
			my1 = (y1 + y2) / 2.0;
			my2 = (y2 + y3) / 2.0;
			*xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
			if (fabsy1y2 > fabsy2y3) {
				*yc = m1 * (*xc - mx1) + my1;
			} else {
				*yc = m2 * (*xc - mx2) + my2;
			}
		}

		dx = x2 - *xc;
		dy = y2 - *yc;
		*rsqr = dx*dx + dy*dy;

		dx = xp - *xc;
		dy = yp - *yc;
		drsqr = dx*dx + dy*dy;

		// Original
		//return((drsqr <= *rsqr) ? TRUE : FALSE);
		// Proposed by Chuck Morris
		return((drsqr - *rsqr) <= AUGE_EPSILON);
	}

	bool DelaunayProcessorImpl::FindEdge(g_int v1, g_int v2)
	{
		d_edge_t* ptr = m_edges;
		for(g_uint i=0; i<m_edge_count; i++,ptr++)
		{
			if((ptr->v1==v1)&&(ptr->v2==v2))
			{
				return true;
			}
			if((ptr->v1==v2)&&(ptr->v2==v1))
			{
				return true;
			}
		}

		return false;
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

	bool DelaunayProcessorImpl::WriteTriangles(const char* outClassName, FeatureWorkspace* poutWorkspace, g_uint srid)
	{
		FeatureClass* ptriFeatureClass = NULL;
		ptriFeatureClass = CreateTriangleFeatureClass(outClassName, poutWorkspace, srid);
		if(ptriFeatureClass==NULL)
		{
			return false;
		}

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
		for(g_uint i=0; i<m_triangle_count; i++)
		//for(g_uint i=0; i<6; i++)
		{
			v1 = m_triangles[i].v1;
			v2 = m_triangles[i].v2;
			v3 = m_triangles[i].v3;

			ptr[0].x = m_vertexes[v1].x;
			ptr[0].y = m_vertexes[v1].y;
			ptr[1].x = m_vertexes[v2].x;
			ptr[1].y = m_vertexes[v2].y;
			ptr[2].x = m_vertexes[v3].x;
			ptr[2].y = m_vertexes[v3].y;
			ptr[3].x = m_vertexes[v1].x;
			ptr[3].y = m_vertexes[v1].y;

			pGeometry = pGeometryFactory->CreateGeometryFromWKB((g_byte*)pWKBPolygon, true);
			pGeoValue = new GValue(pGeometry);
			pFeature->SetValue(AUGE_DEFAULT_GEOM_FIELD, pGeoValue);

			cmd->Insert(pFeature);
		}
		pFeature->Release();
		cmd->Release();
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

	bool DelaunayProcessorImpl::WriteEdges(const char* outClassName, FeatureWorkspace* poutWorkspace, g_uint srid)
	{
		FeatureClass* ptriFeatureClass = NULL;
		ptriFeatureClass = CreateEdgeFeatureClass(outClassName, poutWorkspace, srid);
		if(ptriFeatureClass==NULL)
		{
			return false;
		}

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
		for(g_uint i=0; i<m_edge_count; i++)
		{
			v1 = m_edges[i].v1;
			v2 = m_edges[i].v2;

			ptr[0].x = m_vertexes[v1].x;
			ptr[0].y = m_vertexes[v1].y;
			ptr[1].x = m_vertexes[v2].x;
			ptr[1].y = m_vertexes[v2].y;

			pGeometry = pGeometryFactory->CreateGeometryFromWKB((g_byte*)pWKBLineString, true);
			pGeoValue = new GValue(pGeometry);
			pFeature->SetValue(AUGE_DEFAULT_GEOM_FIELD, pGeoValue);

			cmd->Insert(pFeature);
		}
		pFeature->Release();
		cmd->Release();
		free(pWKBLineString);

		return true;
	}
}
