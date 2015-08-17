#ifndef __AUGE_SYMBOL_MANAGER_IMPL_H__
#define __AUGE_SYMBOL_MANAGER_IMPL_H__

#include "AugeCarto.h"
#include <string>

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
		virtual MarkerSymbol*	GetMarker(const char* name);

		virtual EnumSymbol*		GetLineSymbols();
		virtual EnumSymbol*		GetFillSymbols();

		virtual SimpleMarkerSymbol*	CreateMarkerSymbol(augeMarkerType type);
		virtual SimpleMarkerSymbol*	CreateMarkerSymbol(const char* name);

		virtual LineSymbol*		CreateLineSymbol(augeLineType type);		
		virtual LineSymbol*		CreateLineSymbol(const char* name);

		virtual FillSymbol*		CreateFillSymbol(augeRegionType type);		
		virtual FillSymbol*		CreateFillSymbol(const char* name);
		virtual FillSymbol*		GetFillSymbol(const char* name);

		virtual const char*		GetPath();

	public:
		void			Initialize();
		bool			IsInitialized();

	private:
		void			LoadVectorMarkers();
		void			LoadGraphicSymbols(EnumSymbolImpl* pSymbols, const char* graphic_base);

	private:
		EnumSymbolImpl*	m_marker_symbols;
		EnumSymbolImpl*	m_line_symbols;
		EnumSymbolImpl* m_fill_symbols;

		std::string		m_path;
	};
}

#endif //__AUGE_SYMBOL_MANAGER_IMPL_H__
