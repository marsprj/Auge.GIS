#ifndef __AUGE_GEOMETRY_UTIL_IMPL_H__
#define __AUGE_GEOMETRY_UTIL_IMPL_H__

#include "AugeGeometry.h"

namespace auge
{
	class GeometryUtilImpl : public GeometryUtil
	{
	public:
		GeometryUtilImpl();
		virtual ~GeometryUtilImpl();
	public:
		virtual g_uint	GetWKBSize(const g_uchar* wkb);

	public:
		g_uint  GetWKBSize(LinearRing*            pLinearRing);
		g_uint  GetWKBSize(WKBPoint*              pPoint);
		g_uint  GetWKBSize(WKBLineString*         pLineString);
		g_uint  GetWKBSize(WKBPolygon*            pPolygon);
		g_uint  GetWKBSize(WKBMultiPoint*         pMultiPoint);
		g_uint  GetWKBSize(WKBMultiLineString*    pMultiLineString);
		g_uint  GetWKBSize(WKBMultiPolygon*       pMultiPolygon);
	};
}

#endif //__AUGE_GEOMETRY_UTIL_IMPL_H__
