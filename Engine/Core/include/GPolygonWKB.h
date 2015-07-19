#ifndef __AUGE_POLYGON_WKB_H__
#define __AUGE_POLYGON_WKB_H__

#include "AugeGeometry.h"
#include <string>

namespace auge
{
	class GPolygonWKB : public GPolygon
	{
	public:
		GPolygonWKB();
		virtual ~GPolygonWKB();
	public:
		virtual augeGeometryType	GeometryType();
		virtual const GEnvelope*	Envelope() const;
		virtual const char*			AsText(bool multi=false);
		virtual g_uchar*			AsBinary() const;
		virtual Geometry*			Clone() const;
		virtual void				Centroid(double &x, double &y);
		virtual double				Area() const;
		virtual double				Perimeter() const;
		virtual void				Release();

	public:
		bool	Create(g_uchar *wkb, bool attach);

	private:
		WKBPolygon*	m_pWKBPolygon;
		std::string	m_wkt;
		GEnvelope	m_extent;
		bool		m_attach;
	};
}

#endif //__AUGE_POLYGON_WKB_H__
