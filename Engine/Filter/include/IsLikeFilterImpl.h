#ifndef __AUGE_FILTER_IS_LIKE_IMPL_H__
#define __AUGE_FILTER_IS_LIKE_IMPL_H__

#include "AugeFilter.h"
#include <string>

namespace auge
{
	class IsLikeFilterImpl : public IsLikeFilter
	{
	public:
		IsLikeFilterImpl();
		virtual ~IsLikeFilterImpl();

	public:	
		virtual		Expression*		GetPropertyName();
		virtual		Literal*			GetLiteral();
		virtual		const char*			GetWildCard();
		virtual		const char*			GetSingleChar();
		virtual		const char*			GetEscapeChar();
		virtual		void				Release();

	private:
		Expression	*m_pPropertyName;
		Literal		*m_pLiteral;
		std::string		m_strWildCard;
		std::string		m_strSingleChar;
		std::string		m_strEscapeChar;
	};

}

#endif	//__AUGE_FILTER_IS_LIKE_IMPL_H__