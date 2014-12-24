#ifndef __AUGE_STROKE_IMPL_H__
#define __AUGE_STROKE_IMPL_H__

#include "AugeStyle.h"

namespace auge
{
	class StrokeImpl : public Stroke
	{
	public:
		StrokeImpl();
		//StrokeImpl(Stroke& other);
		StrokeImpl(GColor const& c, float width=1.0);
		virtual ~StrokeImpl();
	public:
		virtual GColor&			GetColor();
		virtual void			SetColor(GColor& color);

		virtual augeLineCap		GetLinecap();
		virtual void			SetLinecap(augeLineCap linecap);

		virtual augeLineJoin	GetLinejoin();
		virtual void			SetLinejoin(augeLineJoin linejoin);

		virtual float			GetOpacity();
		virtual void			SetOpacity(float opacity);

		virtual float			GetWidth();
		virtual void			SetWidth(float width);

		virtual float			GetDashoffset();			
		virtual void			SetDashoffset(float dash_offset);

		virtual GraphicFill*	GetGraphicFill();
		virtual void			SetGraphicFill(GraphicFill* pGraphicFill);

		virtual GraphicStroke*	GetGraphicStroke();
		virtual void			SetGraphicStroke(GraphicStroke* pGraphicFill);

		virtual Stroke&			operator=(const Stroke& rhs);

		virtual void			Release();

	private:
		GColor			m_color;
		float			m_width;
		float			m_opacity;	/* 0.0 - 1.0 */
		augeLineCap		m_line_cap;
		augeLineJoin	m_line_join;
		float			m_dash_offset;

		GraphicFill	*m_pGraphicFill;
		GraphicStroke	*m_pGraphicStroke;
	};

}

#endif //__AUGE_STROKE_IMPL_H__

