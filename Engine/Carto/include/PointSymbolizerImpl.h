#ifndef __AUGE_POINT_SYMBOLIZERE_IMPL_H__
#define __AUGE_POINT_SYMBOLIZERE_IMPL_H__

#include "AugeStyle.h"
#include <string>

namespace auge
{

class PointSymbolizerImpl : public PointSymbolizer
{
public:
	PointSymbolizerImpl();
	virtual ~PointSymbolizerImpl();
public:
	virtual const char*		GetGeometry();
	virtual void			SetGeometry(const char* prop_name);

	virtual void			SetGraphic(Graphic* pGraphic);
	virtual Graphic*		GetGraphic();

	virtual void			SetMarkType(augeMarkerType type);
	virtual augeMarkerType	GetMarkType();

	virtual void			SetStroke(Stroke* pStroke);
	virtual Stroke*			GetStroke();

	virtual void			SetFill(Fill* pFill);
	virtual Fill*			GetFill();

	virtual void			SetOpacity(float opacity);
	virtual float			GetOpacity();

	virtual void			SetSize(float size);
	virtual float			GetSize();

	virtual void			SetRotation(float rotation);
	virtual float			GetRotation();	

	virtual augeSymbolizerType	GetType();	

	virtual void			Release();

private:
	std::string	m_geometry_prop_name;
	Graphic		*m_pGraphic;
	Stroke		*m_pStroke;
	Fill		*m_pFill;
	float		m_opacity;
	float		m_size;
	float		m_rotation;
	augeMarkerType m_mark_type;
};

}

#endif //__AUGE_POINT_SYMBOLIZERE_IMPL_H__

