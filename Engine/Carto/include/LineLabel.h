#ifndef __AUGE_LINE_LABEL_H__
#define __AUGE_LINE_LABEL_H__

#include "Label.h"

namespace auge
{
	class LineLabel : public GLabel
	{
	public:
		LineLabel();
		virtual ~LineLabel();
	public:
		virtual void		Draw(Renderer* pRenderer, Transformation* pTransformation);
		virtual void		ComputePosition(Renderer* pRenderer, Transformation* pTransform);
		//virtual bool		IsCollision(GLabel* other);
		virtual bool		IsVisible(int view_width, int view_height);
	private:
		WKBPoint			m_pos;
		GEnvelope			m_text_extent;
	};
}

#endif //__AUGE_LINE_LABEL_H__
