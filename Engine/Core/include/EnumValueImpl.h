#ifndef __AUGE_ENUM_VALUE_IMPL_H__
#define __AUGE_ENUM_VALUE_IMPL_H__

#include "AugeCore.h"
#include <vector>

namespace auge
{
	class GValue;

	class EnumValueImpl : public EnumValue
	{
	public:
		EnumValueImpl();
		virtual ~EnumValueImpl();
	public:
		virtual void	Reset();
		virtual	GValue*	Next();
		virtual void	Release();

	private:
		void	Add(GValue* pValue);
		void	Cleanup();

	private:
		std::vector<GValue*> m_values;
		std::vector<GValue*>::iterator m_iter;
	};
}

#endif //__AUGE_ENUM_VALUE_IMPL_H__
