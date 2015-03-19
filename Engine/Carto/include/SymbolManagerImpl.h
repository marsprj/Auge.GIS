#ifndef __AUGE_SYMBOL_MANAGER_IMPL_H__
#define __AUGE_SYMBOL_MANAGER_IMPL_H__

#include "AugeCarto.h"

namespace auge
{
	class SymbolManagerImpl : public SymbolManager
	{
	public:
		SymbolManagerImpl();
		virtual ~SymbolManagerImpl();
	public:
		virtual GSymbol*		GetSymbol(g_uint id);
	};
}

#endif //__AUGE_SYMBOL_MANAGER_IMPL_H__
