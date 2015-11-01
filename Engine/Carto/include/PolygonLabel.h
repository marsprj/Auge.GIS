#ifndef __AUGE_POLYGON_LABEL_H__
#define __AUGE_POLYGON_LABEL_H__

#include "Label.h"

namespace auge
{
	class PolygonLabel : public GLabel
	{
	public:
		PolygonLabel();
		virtual ~PolygonLabel();
	public:
		virtual void		Draw(Renderer* pRenderer, Transformation* pTransformation);
		virtual void		ComputePosition(Renderer* pRenderer, Transformation* pTransform);
		//svirtual bool		IsCollision(GLabel* other);
		virtual bool		IsVisible(int view_width, int view_height);

	private:
		void				ComputeCentroidPoint(double &x, double &y);

	private:
		WKBPoint			m_pos;
		GEnvelope			m_text_extent;
	};
}

#endif //__AUGE_POLYGON_LABEL_H__
