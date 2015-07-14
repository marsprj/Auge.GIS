#ifndef __AUGE_ENUM_SYMBOL_IMPL_H__
#define __AUGE_ENUM_SYMBOL_IMPL_H__

#include "AugeSymbol.h"
#include <vector>

namespace auge
{
	class EnumSymbolImpl : public EnumSymbol
	{
	public:
		EnumSymbolImpl();
		virtual ~EnumSymbolImpl();

	public:
		virtual void		Reset();
		virtual Symbol*		Next();
		virtual void		Release();

	public:
		void				Add(Symbol* pSymbol);

	private:
		void	Cleanup();
		
	private:
		std::vector<Symbol*>			m_symbols;
		std::vector<Symbol*>::iterator	m_iter;
	};
}

#endif //__AUGE_ENUM_SYMBOL_IMPL_H__
