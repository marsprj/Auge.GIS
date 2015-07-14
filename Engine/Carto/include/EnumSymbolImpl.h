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
		virtual Symbolizer*	Next();
		virtual void		Release();

	private:
		void	Cleanup();
		
	private:
		std::vector<Symbolizer*>			m_symbols;
		std::vector<Symbolizer*>::iterator	m_iter;
	};
}

#endif //__AUGE_ENUM_SYMBOL_IMPL_H__
