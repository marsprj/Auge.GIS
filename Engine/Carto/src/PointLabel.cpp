#include "PointLabel.h"
#include "AugeGeometry.h"
#include "RendererCairo.h"

namespace auge
{
	PointLabel::PointLabel()
	{
		m_type = augeGTPoint;
	}

	PointLabel::~PointLabel()
	{

	}

	void PointLabel::Draw(Renderer* pRenderer, Transformation* pTransformation)
	{
		int x = m_pos.point.x;
		int y = m_pos.point.y;
		switch(m_pGeometry->GeometryType())
		{
		case augeGTPoint:
			pRenderer->DrawText(GetText(), x, y ,m_pSymbolizer);
			break;
		case augeGTMultiPoint:
			break;
		}
	}

	void PointLabel::ComputePosition(Renderer* pRenderer, Transformation* pTransform)
	{
		int sx=0, sy=0;
		double x=0.0f, y=0.0f;
		m_pGeometry->Centroid(x, y);
		pTransform->ToScreenPoint(x, y, sx, sy);

		
		pRenderer->GetTextExtent((char*)m_wtext.c_str(), m_text_extent);

		m_pos.point.x = sx;
		m_pos.point.y = sy;

		m_pos.point.x += m_pSymbolizer->GetDisplacementX();
		m_pos.point.y += m_pSymbolizer->GetDisplacementY();

		m_pos.point.x -= m_text_extent.GetWidth() * m_pSymbolizer->GetAnchorX();
		m_pos.point.y += m_text_extent.GetHeight()* m_pSymbolizer->GetAnchorY();

		m_text_extent.MoveTo(m_pos.point.x, m_pos.point.y);
		m_text_extent.Inflate(1);
	}

	bool PointLabel::IsCollision(GLabel* other)
	{
		if(other==NULL)
		{
			return false;
		}
		
		switch(other->GetType())
		{
		case augeGTPoint:
			{
				PointLabel* pl = static_cast<PointLabel*>(other);
				return m_text_extent.Intersects(pl->m_text_extent);
			}
		case augeGTLineString:
			break;
		case augeGTPolygon:
			break;
		}


		return true;
	}
}