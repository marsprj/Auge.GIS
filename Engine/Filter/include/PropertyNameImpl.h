#ifndef __AUGE_FILTER_PROPERTY_NAME_IMPL_H__
#define __AUGE_FILTER_PROPERTY_NAME_IMPL_H__

#include "AugeFilter.h"
#include <string>

namespace auge
{
	class PropertyNameImpl : public PropertyName
	{
	public:
		PropertyNameImpl();
		virtual ~PropertyNameImpl();
	public:
		virtual	augeExpressionType	GetType();

		//--------------------------------------------------------------------------
		virtual	void			SetName(const char* szName);
		virtual	const char*		GetName();
		//--------------------------------------------------------------------------

		virtual void			Release();
	private:
		std::string	m_strName;
	};

}

#endif	//__AUGE_FILTER_PROPERTY_NAME_IMPL_H__
