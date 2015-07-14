#ifndef __AUGE_SYMBOL_MANAGER_IMPL_H__
#define __AUGE_SYMBOL_MANAGER_IMPL_H__

#include "AugeCarto.h"

namespace auge
{
	class EnumSymbolImpl;

	class SymbolManagerImpl : public SymbolManager
	{
	public:
		SymbolManagerImpl();
		virtual ~SymbolManagerImpl();

	public:
		virtual EnumSymbol*		GetMarkerSymbols();
		virtual EnumSymbol*		GetLineSymbols();
		virtual EnumSymbol*		GetRegionSymbols();

		virtual MarkerSymbol*	CreateMarkerSymbol(augeMarkerType type);
		virtual MarkerSymbol*	CreateMarkerSymbol(const char* name);

		virtual LineSymbol*		CreateLineSymbol(augeLineType type);		
		virtual LineSymbol*		CreateLineSymbol(const char* name);

	private:
		EnumSymbolImpl*	m_marker_symbols;
		EnumSymbolImpl*	m_line_symbols;
		EnumSymbolImpl* m_region_symbols;
	};
}

#endif //__AUGE_SYMBOL_MANAGER_IMPL_H__
