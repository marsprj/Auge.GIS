#ifndef __AUGE_FILL_IMPL_H__
#define __AUGE_FILL_IMPL_H__

#include "AugeStyle.h"

namespace auge
{
	class FillImpl : public Fill
	{
	public:
		FillImpl();
		virtual ~FillImpl();
	public:
		virtual GColor&		GetColor();
		virtual void		SetColor(GColor& color);	

		virtual float		GetOpacity();
		virtual void		SetOpacity(float opacity);

		virtual void		Release();
	private:
		GColor	m_color;
		float	m_fill_opacity;
		float	m_stroke_opacity;
	};

}

#endif //__AUGE_FILL_IMPL_H__

