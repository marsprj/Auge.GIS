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
		//virtual bool		IsCollision(GLabel* other);
	};
}

#endif //__AUGE_LINE_LABEL_H__
