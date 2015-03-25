#ifndef __AUGE_POINT_LABEL_H__
#define __AUGE_POINT_LABEL_H__

#include "Label.h"
#include "AugeGeometry.h"

namespace auge
{
	class PointLabel : public GLabel
	{
	public:
		PointLabel();
		virtual ~PointLabel();

	public:
		virtual void		Draw(Renderer* pRenderer, Transformation* pTransformation);
		virtual void		ComputePosition(Renderer* pRenderer, Transformation* pTransform);
		virtual void		AdjustPosition(int width, int height);
		virtual bool		IsCollision(GLabel* other);
		
	private:
		WKBPoint			m_pos;
		GEnvelope			m_text_extent;
	};
}

#endif //__AUGE_POINT_LABEL_H__
