#include "GPolygonWKB.h"
#include "AugeCore.h"
#include "WKTWriter.h"

#include <math.h>

namespace auge
{
	GPolygonWKB::GPolygonWKB():
	m_pWKBPolygon(NULL),
		m_attach(false)
	{

	}

	GPolygonWKB::~GPolygonWKB()
	{
		if(!m_attach)
		{
			if(m_pWKBPolygon!=NULL)
			{
				auge_free((g_uchar*)m_pWKBPolygon);
				m_pWKBPolygon = NULL;
			}
		}
	}

	augeGeometryType GPolygonWKB::GeometryType()
	{
		return augeGTPolygon;
	}

	const GEnvelope* GPolygonWKB::Envelope() const
	{
		GPolygonWKB* that = (GPolygonWKB*)this;
		if(!that->m_extent.IsValid())
		{
			Point* pt = NULL;
			int numRings = m_pWKBPolygon->numRings;
			LinearRing* pLinearRing = &(m_pWKBPolygon->rings[0]);
			for(int i=0; i<numRings; i++)
			{	
				int numPoints = pLinearRing->numPoints;
				pt = &(pLinearRing->points[0]);
				for(int j=0; j<numPoints; j++, pt++)
				{
					that->m_extent.m_xmin = that->m_extent.m_xmin < pt->x ? that->m_extent.m_xmin : pt->x;
					that->m_extent.m_xmax = that->m_extent.m_xmax > pt->x ? that->m_extent.m_xmax : pt->x;

					that->m_extent.m_ymin = that->m_extent.m_ymin < pt->y ? that->m_extent.m_ymin : pt->y;
					that->m_extent.m_ymax = that->m_extent.m_ymax > pt->y ? that->m_extent.m_ymax : pt->y;
				}
				pLinearRing = (LinearRing*)pt;
			}
		}
		return &m_extent;
	}

	const char*	GPolygonWKB::AsText(bool multi/*=false*/)
	{
		if(m_wkt.empty())
		{
			WKTWriter writer;
			writer.Write(m_wkt, (g_uchar*)m_pWKBPolygon, multi);
		}
		return m_wkt.c_str();
	}

	g_uchar* GPolygonWKB::AsBinary() const
	{
		return (g_uchar*)m_pWKBPolygon;
	}

	void GPolygonWKB::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	bool GPolygonWKB::Create(g_uchar *wkb, bool attach)
	{
		m_pWKBPolygon = (WKBPolygon*)wkb;
		m_attach = attach;
		return true;
	}

	Geometry* GPolygonWKB::Clone() const
	{
		return NULL;
	}

	void GPolygonWKB::Centroid(double &x, double &y)
	{
		x=0.0f,y=0.0f;
		int point_total = 0;
		Point* pt = NULL;
		int numRings = m_pWKBPolygon->numRings;
		LinearRing* pLinearRing = &(m_pWKBPolygon->rings[0]);
		for(int i=0; i<numRings; i++)
		{	
			int numPoints = pLinearRing->numPoints;
			point_total += numPoints;
			pt = &(pLinearRing->points[0]);
			for(int j=0; j<numPoints; j++, pt++)
			{
				x += pt->x;
				y += pt->y;
			}
			pLinearRing = (LinearRing*)pt;
		}
		x /= point_total;
		y /= point_total;
	}

	double GPolygonWKB::Area() const
	{
		double area = 0.0;
		int i=0, j=0;
		int numPoints = 0;
		int numRings  = m_pWKBPolygon->numRings;
		auge::LinearRing* pLinearRing = NULL;
		auge::Point *pt0 = NULL;
		auge::Point *pt1 = NULL;

		pLinearRing = (auge::LinearRing*)(&(m_pWKBPolygon->rings[0]));

		for(i=0; i<numRings; i++)
		{
			numPoints = pLinearRing->numPoints;
			pt0 = (auge::Point*)(&(pLinearRing->points[0]));
			pt1 = pt0 + 1;

			for(j=1; j<numPoints; j++, pt0++, pt1++)
			{
				double a = (pt1->y + pt0->y) * (pt1->x - pt0->y) / 2.0;
				area += a;
			}
			
			pLinearRing = (auge::LinearRing*)(pt1);
		}

		return area;
	}

	double GPolygonWKB::Perimeter() const
	{
		double length = 0.0;
		int i=0, j=0;
		int numPoints = 0;
		int numRings  = m_pWKBPolygon->numRings;
		auge::LinearRing* pLinearRing = NULL;
		auge::Point *pt0 = NULL;
		auge::Point *pt1 = NULL;

		pLinearRing = (auge::LinearRing*)(&(m_pWKBPolygon->rings[0]));

		for(i=0; i<numRings; i++)
		{
			numPoints = pLinearRing->numPoints;
			pt0 = (auge::Point*)(&(pLinearRing->points[0]));
			pt1 = pt0 + 1;

			for(j=1; j<numPoints; j++, pt0++, pt1++)
			{
				length += sqrt(pow((pt1->x - pt0->x), 2) + pow((pt1->y - pt0->y), 2));
			}

			pLinearRing = (auge::LinearRing*)(pt1);
		}

		return length;
	}

	bool GPolygonWKB::Contain(double x, double y)
	{
		g_uint counter = 0;

		int i=0, j=0;
		int numPoints = 0;
		int numRings  = m_pWKBPolygon->numRings;
		auge::LinearRing* pLinearRing = NULL;
		auge::Point *pt0 = NULL;
		auge::Point *pt1 = NULL;
		double xmax;
		double ymin, ymax;
		double v0, v1;

		pLinearRing = (auge::LinearRing*)(&(m_pWKBPolygon->rings[0]));
		for(i=0; i<numRings; i++)
		{
			numPoints = pLinearRing->numPoints;
			pt0 = (auge::Point*)(&(pLinearRing->points[0]));
			pt1 = pt0 + 1;

			for(j=1; j<numPoints; j++, pt0++, pt1++)
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
						}
					}
				}
			}

			pLinearRing = (auge::LinearRing*)(pt1);
		}

		bool contain = true;
		if(contain==0)
		{
			return false;
		}
		return (counter%2);
	}
}
