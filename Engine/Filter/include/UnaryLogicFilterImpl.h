#ifndef __AUGE_FILTER_UNARY_LOGIC_IMPL_H__
#define __AUGE_FILTER_UNARY_LOGIC_IMPL_H__

#include "AugeFilter.h"
#include <string>

namespace auge
{
	class UnaryLogicFilterImpl : public UnaryLogicFilter
{
public:
	UnaryLogicFilterImpl();
	virtual ~UnaryLogicFilterImpl();

public:	
	virtual		GFilter*			GetFilter() = 0;
	virtual		void				Release();

private:
	GFilter	*m_pFilter;
};

}

#endif	//__AUGE_FILTER_UNARY_LOGIC_IMPL_H__
