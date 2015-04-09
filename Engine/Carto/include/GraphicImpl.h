#ifndef __AUGE_GRAPHIC_IMPL_H__
#define __AUGE_GRAPHIC_IMPL_H__

#include "AugeStyle.h"

namespace auge
{
	class GraphicImpl : public Graphic
	{
	public:
		GraphicImpl();
		virtual ~GraphicImpl();
	public:
		virtual float		GetOpacity();
		virtual void		SetOpacity(float opacity);

		virtual int			GetSize();
		virtual void		SetSize(int size);

		virtual float		GetRotation();
		virtual void		SetRotation(float rotation);

		virtual float		GetAnchor_X();
		virtual float		GetAnchor_Y();
		virtual void		SetAnchor(float x, float y);

		virtual float		GetDisplacement_X();
		virtual float		GetDisplacement_Y();
		virtual void		SetDisplacement(float x, float y);

		virtual Mark*		GetMark();
		virtual bool		SetMark(Mark* pMark);

		virtual ExternalGraphic*	GetExternalGraphic();
		virtual bool		SetExternalGraphic(ExternalGraphic* pExternalGraphic);

	private:
		float m_opacity;
		float m_size;
		float m_rotation;
		float m_anchor_x;
		float m_anchor_y;
		float m_displacement_x;
		float m_displacement_y;
		Mark* m_pMark;
		ExternalGraphic* m_pExternalGraphic;
	};
}

#endif //__AUGE_GRAPHIC_IMPL_H__


