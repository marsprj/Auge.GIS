#include "GMultiPolygonWKB.h"
#include "AugeCore.h"
#include "WKTWriter.h"

#include <math.h>

namespace auge
{
	GMultiPolygonWKB::GMultiPolygonWKB():
	m_pWKBMultiPolygon(NULL),
		m_attach(false)
	{

	}

	GMultiPolygonWKB::~GMultiPolygonWKB()
	{
		if(!m_attach)
		{
			if(m_pWKBMultiPolygon!=NULL)
			{
				auge_free(m_pWKBMultiPolygon);
				m_pWKBMultiPolygon = NULL;
			}
		}
	}

	augeGeometryType GMultiPolygonWKB::GeometryType()
	{
		return augeGTMultiPolygon;
	}

	const GEnvelope* GMultiPolygonWKB::Envelope() const
	{
		GMultiPolygonWKB* that = (GMultiPolygonWKB*)this;

		Point* pt = NULL;
		int numPolygons = m_pWKBMultiPolygon->numPolygons;
		WKBPolygon* pWKBPolygon = &(m_pWKBMultiPolygon->polygons[0]);
		for(int i=0; i<numPolygons; i++)
		{
			int numRings = pWKBPolygon->numRings;
			LinearRing* pLinearRing = &(pWKBPolygon->rings[0]);
			for(int j=0; j<numRings; j++)
			{	
				int numPoints = pLinearRing->numPoints;
				pt = &(pLinearRing->points[0]);
				for(int k=0; k<numPoints; k++, pt++)
				{
					that->m_extent.m_xmin = that->m_extent.m_xmin < pt->x ? that->m_extent.m_xmin : pt->x;
					that->m_extent.m_xmax = that->m_extent.m_xmax > pt->x ? that->m_extent.m_xmax : pt->x;

					that->m_extent.m_ymin = that->m_extent.m_ymin < pt->y ? that->m_extent.m_ymin : pt->y;
					that->m_extent.m_ymax = that->m_extent.m_ymax > pt->y ? that->m_extent.m_ymax : pt->y;
				}
				pLinearRing = (LinearRing*)pt;
			}
			pWKBPolygon = (WKBPolygon*)pt;
		}
		return &m_extent;
	}

	const char*	GMultiPolygonWKB::AsText(bool multi/*=false*/)
	{
		if(m_wkt.empty())
		{
			WKTWriter writer;
			writer.Write(m_wkt, (g_uchar*)m_pWKBMultiPolygon);
		}
		return m_wkt.c_str();
	}

	g_uchar* GMultiPolygonWKB::AsBinary() const
	{
		return (g_uchar*)m_pWKBMultiPolygon;
	}

	void GMultiPolygonWKB::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	bool GMultiPolygonWKB::Create(g_uchar *wkb, bool attach)
	{
		m_pWKBMultiPolygon = (WKBMultiPolygon*)wkb;
		m_attach = attach;
		return true;
	}

	Geometry* GMultiPolygonWKB::Clone() const
	{
		return NULL;
	}

	void GMultiPolygonWKB::Centroid(double &x, double &y)
	{
		x=0.0f,y=0.0f;
		int point_total = 0;
		Point* pt = NULL;
		int numPolygons = m_pWKBMultiPolygon->numPolygons;
		WKBPolygon* pWKBPolygon = &(m_pWKBMultiPolygon->polygons[0]);
		for(int i=0; i<numPolygons; i++)
		{
			int numRings = pWKBPolygon->numRings;
			LinearRing* pLinearRing = &(pWKBPolygon->rings[0]);
			for(int j=0; j<numRings; j++)
			{	
				int numPoints = pLinearRing->numPoints;
				point_total += numPoints;
				pt = &(pLinearRing->points[0]);
				for(int k=0; k<numPoints; k++, pt++)
				{
					x += pt->x;
					y += pt->y;
				}
				pLinearRing = (LinearRing*)pt;
			}
			pWKBPolygon = (WKBPolygon*)pt;
		}
		
		x /= point_total;
		y /= point_total;
	}

	double GMultiPolygonWKB::Area() const
	{
		double area=0.0;
		auge::Point* pt0 = NULL;
		auge::Point* pt1 = NULL; 

		int numPolygons = m_pWKBMultiPolygon->numPolygons;
		WKBPolygon* pWKBPolygon = &(m_pWKBMultiPolygon->polygons[0]);
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
					double a = (pt1->y + pt0->y) * (pt1->x - pt0->x) / 2.0;
					area += a;
				}
				pLinearRing = (LinearRing*)pt1;
			}
			pWKBPolygon = (WKBPolygon*)pt1;
		}
		return area;
	}

	double GMultiPolygonWKB::Perimeter() const
	{
		double length=0.0;
		auge::Point* pt0 = NULL;
		auge::Point* pt1 = NULL; 

		int numPolygons = m_pWKBMultiPolygon->numPolygons;
		WKBPolygon* pWKBPolygon = &(m_pWKBMultiPolygon->polygons[0]);
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
					length += sqrt(pow((pt1->x - pt0->x), 2) + pow((pt1->y - pt0->y), 2));
				}
				pLinearRing = (LinearRing*)pt1;
			}
			pWKBPolygon = (WKBPolygon*)pt1;
		}
		return length;
	}

	bool GMultiPolygonWKB::Contain(double x, double y)
	{
		g_uint counter = 0;
		auge::Point* pt0 = NULL;
		auge::Point* pt1 = NULL; 
		double xmax;
		double ymin, ymax;
		double v0, v1;

		int numPolygons = m_pWKBMultiPolygon->numPolygons;
		WKBPolygon* pWKBPolygon = &(m_pWKBMultiPolygon->polygons[0]);
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
					v0 = y - pt0->y;
					v1 = y - pt1->y;
					if((v0==0.0) && (v1==0.0))
					{	//horizonal segment
						counter++;
					}
					else
					{
						ymin = g_min(pt0->y, pt1->y);
						ymax = g_max(pt0->y, pt1->y);
						v0 = y - ymin;
						v1 = y - ymax;

						if( v0>0.0 && v1<0.0)
						{
							// x locates between y0 and y1, then x may be accross the segment [pt0, pt1];
							xmax = g_max(pt0->x, pt1->x);
							if(xmax > x)
							{	// intersection point locates the right side of x

								double xx = 0.0;
								double s1, s2;
								double height = 0.0;
								s1 = y - pt0->y;
								s2 = pt1->y - y;
								if(fabs(s1)<fabs(s2))
								{
									xx = (pt0->x-pt1->x) * ((y-pt1->y) / (pt0->y-pt1->y)) + pt1->x;
								}
								else
								{
									xx = (pt1->x-pt0->x) * (y-pt0->y) / (pt1->y-pt0->y) + pt0->x;
								}
								if(xx>x)
								{
									counter++;
								}

								//counter++;
							}
						}
					}
				}
				pLinearRing = (LinearRing*)pt1;
			}
			pWKBPolygon = (WKBPolygon*)pt1;
		}

		bool contain = true;
		if(contain==0)
		{
			return false;
		}
		return (counter%2);
	}
}
