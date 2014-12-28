#ifndef __AUGE_EXPRESSION_LITERAL_IMPL_H__
#define __AUGE_EXPRESSION_LITERAL_IMPL_H__

#include "AugeFilter.h"
#include <string>

namespace auge
{
	class LiteralImpl : public Literal
	{
	public:
		LiteralImpl();
		virtual ~LiteralImpl();
	public:
		virtual	augeExpressionType		GetType();

		virtual		bool				SetValue(GValue* pValue);
		virtual		GValue*			GetValue();
		virtual		void				Release();
	private:
		GValue *m_pValue;
	};

}

#endif	//__AUGE_EXPRESSION_LITERAL_IMPL_H__
