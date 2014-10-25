#ifndef __AUGE_TEXT_SYMBOLIZER_H__
#define __AUGE_TEXT_SYMBOLIZER_H__

#include "Base.h"
#include "Color.h"
#include <string>

namespace auge
{
	typedef enum FontStyle
	{
		augeFontStyleNormal	= 0,
		augeFontStyleItalic	= 1,
		augeFontStyleOblique= 2,
	}FontStyle;

	typedef enum FontWeight
	{
		augeFontWeightNormal= 0,
		augeFontWeightBold	= 1
	}FontWeight;

	class Symbolizer;

	class AUGE_API TextSymbolizer : public Symbolizer
	{
	public:
		TextSymbolizer();
		virtual ~TextSymbolizer();

	public:
		virtual	SymbolizerType	GetType(){return augeSymbolText;}

	public:
		bool		fill;
		bool		outline;

		std::string	field;
		std::string	font;		
		size_t		size;
		FontStyle	slant;
		FontWeight	weight;

		Color		color;
		Color		outline_color;
		double		outline_width;

		double		offset_x;
		double		offset_y;

	};

}//namespace

#endif //__AUGE_TEXT_SYMBOLIZER_H__
