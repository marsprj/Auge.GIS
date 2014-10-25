#ifndef __AUGE_UTIL_H__
#define __AUGE_UTIL_H__

#include "Field.h"
#include "Geometry.h"
#include <libpq-fe.h>

namespace auge
{
	class Util
	{
	public:
		Util();
		~Util();
	public:
		static FieldType	ParseFieldType(Oid otype);
		static GeometryType	ParseGeometryType(const char* stype);
	};
}

#endif //__AUGE_UTIL_H__
