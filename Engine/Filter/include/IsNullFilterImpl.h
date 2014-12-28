#ifndef __AUGE_FILTER_IS_NULL_IMPL_H__
#define __AUGE_FILTER_IS_NULL_IMPL_H__

#include "AugeFilter.h"
#include <string>

namespace auge
{
	class IsNullFilterImpl : public IsNullFilter
	{
	public:
		IsNullFilterImpl();
		virtual ~IsNullFilterImpl();

	public:	
		virtual		Expression*		GetPropertyName();
		virtual		void				Release();

	private:
		Expression	*m_pPropertyName;
	};

}

#endif	//__AUGE_FILTER_IS_NULL_IMPL_H__