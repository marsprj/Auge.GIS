#ifndef __AUGE_POLYGON_SYMBOLIZER_H__
#define __AUGE_POLYGON_SYMBOLIZER_H__

#include "Base.h"
#include "Color.h"

namespace auge
{
	class Symbolizer;

	class AUGE_API PolygonSymbolizer : public Symbolizer
	{
	public:
		PolygonSymbolizer();
		virtual ~PolygonSymbolizer();

	public:
		virtual	SymbolizerType	GetType(){return augeSymbolPolygon;}

	public:
		bool	 fill;
		bool	 outline;
		double	 radius;

		Color	 fill_color;

		Color	 outline_color;
		double	 outline_width;
		LineCap	 outline_cap;
		LineJoin outline_join;
	};

}//namespace

#endif //__AUGE_POLYGON_SYMBOLIZER_H__