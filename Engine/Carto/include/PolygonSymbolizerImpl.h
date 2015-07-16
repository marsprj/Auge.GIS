#ifndef __AUGE_POLYGON_SYMBOLIZERE_IMPL_H__
#define __AUGE_POLYGON_SYMBOLIZERE_IMPL_H__

#include "AugeStyle.h"

namespace auge
{
	class PolygonSymbolizerImpl : public PolygonSymbolizer
	{
	public:
		PolygonSymbolizerImpl();
		virtual ~PolygonSymbolizerImpl();
	public:
		virtual Fill*		GetFill();
		virtual void		SetFill(Fill* pFill);

		virtual Stroke*		GetStroke();
		virtual void		SetStroke(Stroke* pStroke);

		virtual void		SetOpacity(float opacity);
		virtual float		GetOpacity();

		virtual float		GetDisplacement_X();
		virtual float		GetDisplacement_Y();
		virtual void		SetDisplacement(float x, float y);

		virtual const char*	GetGemetry();
		virtual void		SetGeometry(const char* prop_name);

		virtual void		Release();

		virtual augeSymbolizerType GetType();

		virtual void			SetSymbol(RegionSymbol* pSymbol);
		virtual RegionSymbol*	GetSymbol();

	private:
		Fill		*m_pFill;
		Stroke		*m_pStroke;
		float		m_opacity;
		float		m_displacement_X;
		float		m_displacement_Y;
		int			m_perpendicular_offset;
		const char	*m_geometry_prop_name;

		RegionSymbol	*m_pSymbol;
	};

}

#endif //__AUGE_POLYGON_SYMBOLIZERE_IMPL_H__

