#include "LineLabel.h"
#include "AugeGeometry.h"
#include "RendererCairo.h"

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
		int x = m_pos.point.x;
		int y = m_pos.point.y;
		switch(m_pGeometry->GeometryType())
		{
		case augeGTLineString:
			pRenderer->DrawText(GetText(), x, y ,m_pSymbolizer);
			break;
		case augeGTMultiLineString:
			pRenderer->DrawText(GetText(), x, y ,m_pSymbolizer);
			break;
		}
	}

	bool LineLabel::IsVisible(int view_width, int view_height)
	{
		if(m_text_extent.m_xmax<0)
		{
			return false;
		}
		if(m_text_extent.m_xmin>view_width)
		{
			return false;
		}
		if(m_text_extent.m_ymax<0)
		{
			return false;
		}
		if(m_text_extent.m_ymin>view_width)
		{
			return false;
		}
		return true;
	}

	void LineLabel::ComputePosition(Renderer* pRenderer, Transformation* pTransform)
	{
		int sx=0, sy=0;
		double x=0.0f, y=0.0f;
		m_pGeometry->Centroid(x, y);
		//ComputeCentroidPoint(x, y);
		pTransform->ToScreenPoint(x, y, sx, sy);

		pRenderer->GetTextExtent((char*)m_wtext.c_str(), m_text_extent);

		m_pos.point.x = sx - m_text_extent.GetWidth() / 2;
		m_pos.point.y = sy - m_text_extent.GetHeight() / 2;

		m_pos.point.x += m_pSymbolizer->GetDisplacementX();
		m_pos.point.y += m_pSymbolizer->GetDisplacementY();

		m_pos.point.x -= m_text_extent.GetWidth() * m_pSymbolizer->GetAnchorX();
		m_pos.point.y += m_text_extent.GetHeight()* m_pSymbolizer->GetAnchorY();

		m_text_extent.MoveTo(m_pos.point.x, m_pos.point.y);
		m_text_extent.Inflate(1);
	}

}