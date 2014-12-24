#ifndef __AUGE_TEXT_SYMBOLIZERE_IMPL_H__
#define __AUGE_TEXT_SYMBOLIZERE_IMPL_H__

#include "AugeStyle.h"
#include <string>

namespace auge
{

	class TextSymbolizerImpl : public TextSymbolizer
	{
	public:
		TextSymbolizerImpl();
		virtual ~TextSymbolizerImpl();
	public:

		virtual Font*		GetFont();
		virtual bool		SetFont(Font* pFont);

		virtual Fill*		GetFill();
		virtual bool		SetFill(Fill* pFill);

		virtual Stroke*		GetStroke();
		virtual bool		SetStroke(Stroke* pStroke);

		virtual const char*	GetGeometry();
		virtual bool		SetGeometry(const char* prop_name);

		virtual const char*	GetLabel();
		virtual bool		SetLabel(const char* prop_name);

		virtual const char*	GetLabelText();
		virtual bool		SetLabelText(const char* text);

		virtual double		GetAnchorX();
		virtual double		GetAnchorY();
		virtual void		SetAnchor(double x, double y);

		virtual double		GetDisplacementX();
		virtual double		GetDisplacementY();
		virtual void		SetDisplacement(double x, double y);

		virtual double		GetRotation();
		virtual void		SetRotation(double rotation);
		virtual const char*	GetRotationField();
		virtual void		SetRotationFiled(const char* prop_name);

		virtual void		Release();

		virtual augeSymbolizerType	GetType();	
	private:
		std::string	m_geometry_prop_name;
		std::string	m_label_prop_name;
		std::string m_label_text;

		double		m_anchor_x;
		double		m_anchor_y;

		double		m_displace_x;
		double		m_displace_y;

		double		m_rotation;
		std::string m_rotation_prop;

		Font		*m_pFont;
		Fill		*m_pFill;
		Stroke		*m_pStroke;

	};

}

#endif //__AUGE_TEXT_SYMBOLIZERE_IMPL_H__

