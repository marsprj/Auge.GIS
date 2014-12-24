#ifndef __AUGE_FONT_IMPL_H__
#define __AUGE_FONT_IMPL_H__

#include "AugeStyle.h"
#include <string>

namespace auge
{
	class FontImpl : public Font
	{
	public:
		FontImpl();
		virtual ~FontImpl();
	public:
		virtual const char*		GetFamily();
		virtual bool			SetFamily(const char* szFamily);

		virtual augeFontStyle	GetStyle();
		virtual void			SetStyle(augeFontStyle style);

		virtual augeFontWeight	GetWeight();
		virtual void			SetWeight(augeFontWeight weight);

		virtual float			GetSize();
		virtual void			SetSize(float size);

		virtual void			Release();

	private:
		std::string		m_family;
		augeFontStyle	m_style;
		augeFontWeight	m_weght;
		float			m_size;
	};

}

#endif //__AUGE_FONT_IMPL_H__

