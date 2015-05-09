#include "AugeStyle.h"
#include "TextSymbolizerImpl.h"
#include "FontImpl.h"
#include "string.h"


namespace auge 
{
	TextSymbolizerImpl::TextSymbolizerImpl():
	m_pFont(new FontImpl),
	m_pFill(NULL),
	m_pStroke(NULL),
	//m_anchor(1),
	m_anchor_x(0.0f),
	m_anchor_y(1.0f),
	m_displace_x(0.0f),
	m_displace_y(0.0f),
	m_rotation(0.0f)
	{

	}

	TextSymbolizerImpl::~TextSymbolizerImpl()
	{
		if(m_pFont!=NULL)
		{
			m_pFont->Release();
			m_pFont = NULL;
		}
		if(m_pFill!=NULL)
		{
			m_pFill->Release();
			m_pFill = NULL;
		}
		if(m_pStroke!=NULL)
		{
			m_pStroke->Release();
			m_pStroke = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void TextSymbolizerImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	Font*	TextSymbolizerImpl::GetFont()
	{
		return m_pFont;
	}

	bool TextSymbolizerImpl::SetFont(Font* pFont)
	{
		if(m_pFont!=NULL)
		{
			m_pFont->Release();
			m_pFont = NULL;
		}
		m_pFont = pFont;
		return true;
	}

	Fill* TextSymbolizerImpl::GetFill()
	{
		return m_pFill;
	}

	bool TextSymbolizerImpl::SetFill(Fill* pFill)
	{
		if(m_pFill!=NULL)
		{
			m_pFill->Release();
			m_pFill = NULL;
		}
		m_pFill = pFill;
		return true;
	}

	Stroke*	TextSymbolizerImpl::GetStroke()
	{
		return m_pStroke;
	}

	bool TextSymbolizerImpl::SetStroke(Stroke* pStroke)
	{
		if(m_pStroke!=NULL)
		{
			m_pStroke->Release();
			m_pStroke = NULL;
		}
		m_pStroke = pStroke;
		return true;
	}

	const char*	TextSymbolizerImpl::GetGeometry()
	{
		return m_geometry_prop_name.c_str();
	}

	bool TextSymbolizerImpl::SetGeometry(const char* prop_name)
	{
		if(prop_name==NULL)
		{
			m_geometry_prop_name.clear();
		}
		m_geometry_prop_name = prop_name;
		return false;
	}

	const char*	TextSymbolizerImpl::GetLabel()
	{
		return m_label_prop_name.c_str();
	}

	bool TextSymbolizerImpl::SetLabel(const char* prop_name)
	{
		if(prop_name==NULL)
		{
			m_label_prop_name.clear();
			return false;
		}

		m_label_prop_name = prop_name;
		return true;
	}

	const char*	TextSymbolizerImpl::GetLabelText()
	{
		return m_label_text.c_str();
	}

	bool TextSymbolizerImpl::SetLabelText(const char* text)
	{
		if(text==NULL)
		{
			return false;
		}
		m_label_text = text;
		return true;
	}

	//g_uint TextSymbolizerImpl::GetAnchor()
	//{
	//	return m_anchor;
	//}
	//
	double TextSymbolizerImpl::GetAnchorX()
	{
		return m_anchor_x;
	}

	double TextSymbolizerImpl::GetAnchorY()
	{
		return m_anchor_y;
	}

	void TextSymbolizerImpl::SetAnchor(double x, double y)
	{
		m_anchor_x = x;
		m_anchor_y = y;
	}

	double TextSymbolizerImpl::GetDisplacementX()
	{
		return m_displace_y;
	}

	double TextSymbolizerImpl::GetDisplacementY()
	{
		return m_displace_y;
	}

	void TextSymbolizerImpl::SetDisplacement(double x, double y)
	{
		m_displace_x = x;
		m_displace_y = y;
	}

	double TextSymbolizerImpl::GetRotation()
	{
		return m_rotation;
	}

	void TextSymbolizerImpl::SetRotation(double rotation)
	{
		m_rotation = rotation;
	}

	const char*	TextSymbolizerImpl::GetRotationField()
	{
		return m_rotation_prop.c_str();
	}

	void TextSymbolizerImpl::SetRotationFiled(const char* prop_name)
	{
		if(prop_name==NULL)
		{
			m_rotation_prop.clear();
		}
		m_rotation_prop = prop_name;
	}

	augeSymbolizerType TextSymbolizerImpl::GetType()
	{
		return augeSymbolText;
	}

}
