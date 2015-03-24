#ifndef __AUGE_MULTI_POINT_WKB_H__
#define __AUGE_MULTI_POINT_WKB_H__

#include "AugeGeometry.h"
#include <string>

namespace auge
{
	class GMultiPointWKB : public GMultiPoint
	{
	public:
		GMultiPointWKB();
		virtual ~GMultiPointWKB();
	public:
		virtual augeGeometryType	GeometryType();
		virtual const GEnvelope*	Envelope() const;
		virtual const char*			AsText(bool multi=false);
		virtual g_uchar*			AsBinary() const;
		virtual Geometry*			Clone() const;
		virtual void				Centroid(double &x, double &y);
		virtual void				Release();

	public:
		bool	Create(g_uchar *wkb, bool attach);

	private:
		WKBMultiPoint*	m_pWKBMultiPoint;
		std::string		m_wkt;
		GEnvelope		m_extent;
		bool			m_attach;
	};
}

#endif //__AUGE_MULTI_POINT_WKB_H__
