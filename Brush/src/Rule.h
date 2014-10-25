#ifndef __AUGE_RULE_H__
#define __AUGE_RULE_H__

#include "Base.h"

namespace auge
{
	class Symbolizer;
	class Filter;

	class AUGE_API Rule : public Base
	{
	public:
		Rule();
		virtual ~Rule();

	public:
		Symbolizer*		GetSymbolizer();
		void			SetSymbolizer(Symbolizer *pSymbolizer);

		Filter*			GetFilter();
		void			SetFilter(Filter* pFilter);

	private:
		Symbolizer*		m_pSymbolizer;
		Filter*			m_pFilter;
	};

}//namespace

#endif //__AUGE_RULE_H__