#ifndef __AUGE_GML_WRITER_IMPL_H__
#define __AUGE_GML_WRITER_IMPL_H__

#include "AugeGeometry.h"

namespace auge
{
	class GMLWriterImpl : public GMLWriter
	{
	public:
		GMLWriterImpl();
		virtual ~GMLWriterImpl();

	public:
		virtual RESULTCODE	Write(XElement* pxParent, const g_uchar* wkb,  g_int srid, const char* version);
		virtual RESULTCODE	Write(XElement* pxParent, Geometry* pGeometry, g_int srid, const char* version);
		virtual void		Release();

	private:
		RESULTCODE AsGML(XElement* pxParent,WKBPoint* pWKBPoint,			g_int srid);
		RESULTCODE AsGML(XElement* pxParent,WKBLineString* pWKBLineString,	g_int srid);
		RESULTCODE AsGML(XElement* pxParent,WKBPolygon* pWKBPolygon,		g_int srid);
		RESULTCODE AsGML(XElement* pxParent,WKBMultiPoint* pWKBMultiPoint,	g_int srid);
		RESULTCODE AsGML(XElement* pxParent,WKBMultiLineString* pWKBMultiLineString, g_int srid);
		RESULTCODE AsGML(XElement* pxParent,WKBMultiPolygon* pWKBMultiPolygon, g_int srid);
		RESULTCODE AsGML(XElement* pxParent,LinearRing* pLinearRing);
		RESULTCODE AsGML(XElement* pxParent,WKBLineString* pLineString);
		RESULTCODE AsGML(XElement* pxParent,WKBPolygon* pWKBPolygon);
		RESULTCODE AsCoordinates(XElement* pxParent,const char* coordinates);

	private:
		int		FindOutBoundIndex(WKBPolygon* pWKBPolygon);
		void	GetBound(LinearRing* pLinearRing, int dimension, double& dmin, double& dmax);

		int		CalWKBLinearRingLen(LinearRing* pLinearRing);
		int		CalWKBLineStringLen(WKBLineString* pWKBLineString);
		int		CalWKBPolygonLen(WKBPolygon* pWKBPolygon);
	};
}

#endif //__AUGE_GML_WRITER_IMPL_H__
