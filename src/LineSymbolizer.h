#ifndef __AUGE_LINE_SYMBOLIZER_H__
#define __AUGE_LINE_SYMBOLIZER_H__

#include "Symbolizer.h"
#include "Color.h"

namespace auge
{
	class Symbolizer;

	class AUGE_API LineSymbolizer : public Symbolizer
	{
	public:
		LineSymbolizer();
		virtual ~LineSymbolizer();

	public:
		virtual	SymbolizerType	GetType(){return augeSymbolLine;}

	public:
		Color	 outline_color;
		double	 outline_width;
		LineCap	 outline_cap;
		LineJoin outline_join;
	};

}//namespace

#endif //__AUGE_LINE_SYMBOLIZER_H__
