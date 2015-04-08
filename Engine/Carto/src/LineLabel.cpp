#include "LineLabel.h"

namespace auge
{
	LineLabel::LineLabel()
	{
		m_type = augeGTLineString;
	}

	LineLabel::~LineLabel()
	{

	}

	void LineLabel::Draw(Renderer* pRenderer, Transformation* pTransformation)
	{

	}

	bool LineLabel::IsVisible(int view_width, int view_height)
	{
		return true;
	}
}