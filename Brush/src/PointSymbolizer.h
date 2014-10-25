#ifndef __AUGE_POINT_SYMBOLIZER_H__
#define __AUGE_POINT_SYMBOLIZER_H__

#include "Color.h"
#include "Symbolizer.h"

namespace auge
{
	class AUGE_API PointSymbolizer : public Symbolizer
	{
	public:
		PointSymbolizer();
		virtual ~PointSymbolizer();

	public:
		virtual	SymbolizerType	GetType(){return augeSymbolPoint;}

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

#endif //__AUGE_POINT_SYMBOLIZER_H__
