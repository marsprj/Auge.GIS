#ifndef __AUGE_ENUM_FILTER_IMPL_H__
#define __AUGE_ENUM_FILTER_IMPL_H__

#include "AugeFilter.h"
#include <vector>

namespace auge
{
	class EnumFilterImpl : public EnumFilter
	{
	public:
		EnumFilterImpl();
		virtual ~EnumFilterImpl();
	public:
		virtual		GFilter*			Next();
		virtual		void				Reset();
		virtual		void				Release();	

	public:
		bool		AddFilter(GFilter* pFilter);

	private:
		void		Cleanup();

	private:
		typedef	std::vector<GFilter*>	FILTER_VECTOR;
		FILTER_VECTOR			m_filters;
		FILTER_VECTOR::iterator	m_iter;
	};

}

#endif	//__AUGE_ENUM_FILTER_IMPL_H__
