#include "PolygonLabel.h"

namespace auge
{
	PolygonLabel::PolygonLabel()	
	{
		m_type = augeGTPoint;
	}

	PolygonLabel::~PolygonLabel()
	{

	}

	void PolygonLabel::Draw(Renderer* pRenderer, Transformation* pTransformation)
	{

	}

	bool PolygonLabel::IsVisible(int view_width, int view_height)
	{
		return true;
	}
}