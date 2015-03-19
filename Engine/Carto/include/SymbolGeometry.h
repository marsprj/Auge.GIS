#ifndef __AUGE_SYMBOL_GEOMETRY_H__
#define __AUGE_SYMBOL_GEOMETRY_H__

#include "AugeCarto.h"

namespace auge
{
	class SymbolGeometry : public GSymbol
	{
	public:
		SymbolGeometry();
		virtual ~SymbolGeometry();
	public:
		virtual void	Release();
	};
}

#endif //__AUGE_SYMBOL_GEOMETRY_H__