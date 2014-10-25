#include "Symbolizer.h"
#include "TextSymbolizer.h"

namespace auge
{

	TextSymbolizer::TextSymbolizer():
	fill(true),
	outline(true),
	font("Arial"),
	size(12),
	slant(augeFontStyleNormal),
	weight(augeFontWeightNormal),
	offset_x(0.0f),
	offset_y(0.0f),
	outline_width(1.0f)
	{

	}

	TextSymbolizer::~TextSymbolizer()
	{

	}
}