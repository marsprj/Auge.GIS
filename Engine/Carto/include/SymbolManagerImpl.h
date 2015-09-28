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

		virtual RESULTCODE		AddMarkerSymbol(const char* name, const char* path);
		virtual RESULTCODE		AddFillSymbol(const char* name, const char* path);

		virtual SimpleMarkerSymbol*	CreateMarkerSymbol(augeMarkerType type);
		virtual SimpleMarkerSymbol*	CreateMarkerSymbol(const char* name);

		virtual LineSymbol*		CreateLineSymbol(augeLineType type);		
		virtual LineSymbol*		CreateLineSymbol(const char* name);

		virtual FillSymbol*		CreateFillSymbol(augeRegionType type);		
		virtual FillSymbol*		CreateFillSymbol(const char* name);
		virtual FillSymbol*		GetFillSymbol(const char* name);

		virtual const char*		GetPath();

		virtual	RESULTCODE		Initialize(GConnection* pConnection);
	public:
		
		bool			IsInitialized();

	private:
		void			LoadVectorMarkers();
		void			LoadGraphicMarkerSymbols(EnumSymbolImpl* pSymbols, const char* graphic_base);
		void			LoadGraphicFillSymbols(EnumSymbolImpl* pSymbols, const char* graphic_base);
		RESULTCODE		CreateSymbolTable();

	private:
		GConnection*	m_pConnection;
		EnumSymbolImpl*	m_marker_symbols;
		EnumSymbolImpl*	m_line_symbols;
		EnumSymbolImpl* m_fill_symbols;

		std::string		m_path;
	};
}

#endif //__AUGE_SYMBOL_MANAGER_IMPL_H__
