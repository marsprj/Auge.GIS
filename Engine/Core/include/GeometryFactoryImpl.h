#ifndef __AUGE_GEOMETRY_FACTORY_IMPL_H__
#define __AUGE_GEOMETRY_FACTORY_IMPL_H__

#include "AugeGeometry.h"

namespace auge
{
	class GeometryFactoryImpl : public GeometryFactory
	{
	public:
		GeometryFactoryImpl();
		virtual ~GeometryFactoryImpl();
	public:
		virtual	GPoint*				CeatePoint(double x, double y);
		virtual Geometry*			CreateGeometryFromWKB(g_uchar* wkb, bool attach=false);
		virtual Geometry*			CreateMultiPolygonFromWKB(g_uchar* wkb);
		virtual Geometry*			CreateGeometryFromWKT(const char* wkt);
		virtual Geometry*			CreateGeometryFromGML(const char* gml);

		virtual augeGeometryType	DecodeGeometryType(const char* type);
		virtual const char*			Encode(augeGeometryType type);

		virtual	GMLWriter*			CreateGMLWriter();
		virtual GMLReader*			CreateGMLReader();
	};
}

#endif //__AUGE_GEOMETRY_FACTORY_IMPL_H__
