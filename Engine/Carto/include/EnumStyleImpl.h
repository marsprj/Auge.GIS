#ifndef __AUGE_ENUM_STYLE_IMPL_H__
#define __AUGE_ENUM_STYLE_IMPL_H__

#include "AugeCarto.h"
#include <vector>

namespace auge
{
	class Style;

	class EnumStyleImpl : public EnumStyle
	{
	public:
		EnumStyleImpl();
		virtual ~EnumStyleImpl();
	public:
		virtual void	Reset();
		virtual Style*	Next();
		virtual void	Release();

	public:
		void	Add(Style* pStyle);
		void	Cleanup();

	private:
		std::vector<Style*> m_styles;
		std::vector<Style*>::iterator m_iter;
	};
}

#endif //__AUGE_ENUM_STYLE_IMPL_H__
