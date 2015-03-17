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
		virtual Geometry*			CreateGeometryFromWKB(g_uchar* wkb, bool attach=false);
		virtual Geometry*			CreateGeometryFromWKT(const char* wkt);

		virtual augeGeometryType	DecodeGeometryType(const char* type);
		virtual const char*			Encode(augeGeometryType type);

		virtual	GMLWriter*			CreateGMLWriter();
	};
}

#endif //__AUGE_GEOMETRY_FACTORY_IMPL_H__
