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
		//svirtual bool		IsCollision(GLabel* other);
	};
}

#endif //__AUGE_POLYGON_LABEL_H__
