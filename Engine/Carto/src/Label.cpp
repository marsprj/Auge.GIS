#include "Label.h"
#include "AugeCore.h"
#include "AugeCarto.h"
#include "AugeGeometry.h"

namespace auge
{
	GLabel::GLabel():
	m_pGeometry(NULL),
	m_pSymbolizer(NULL),
	m_type(augeGTPoint)
	{

	}

	GLabel::~GLabel()
	{
		if(m_pGeometry!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pGeometry);
		}
		if(m_pSymbolizer!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pSymbolizer);
		}
	}

	void GLabel::SetText(const char* text)
	{
		if(text==NULL)
		{
			m_text.clear();
			m_wtext.clear();
		}
		else
		{
			m_text = text;
			m_wtext = (wchar_t*)auge_encoding_convert("GBK","UTF-8",text, strlen(text));
		}
	}

	augeGeometryType GLabel::GetType()
	{
		return m_type;
	}

	void GLabel::SetGeometry(Geometry* pGeometry)
	{
		if(m_pGeometry)
		{
			AUGE_SAFE_RELEASE(m_pGeometry);
		}
		m_pGeometry = pGeometry;
		if(m_pGeometry)
		{
			m_pGeometry->AddRef();
		}
	}

	void GLabel::SetSymbolizer(TextSymbolizer* pSymbolizer)
	{
		if(m_pSymbolizer)
		{
			AUGE_SAFE_RELEASE(m_pSymbolizer);
		}
		m_pSymbolizer = pSymbolizer;
		if(m_pSymbolizer)
		{
			m_pSymbolizer->AddRef();
		}
	}

	const char*	GLabel::GetText()
	{
		return m_text.c_str();
	}

	const g_char* GLabel::GetTextW()
	{
		return (g_char*)m_wtext.c_str();
	}

	Geometry* GLabel::GetGeometry()
	{
		return m_pGeometry;
	}

	TextSymbolizer* GLabel::GetSymbolizer()
	{
		return m_pSymbolizer;
	}

	void GLabel::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	void GLabel::Draw(Renderer* pRenderer, Transformation* pTransformation)
	{

	}

	void GLabel::ComputePosition(Renderer* pRenderer, Transformation* pTransform)
	{
		
	}

	bool GLabel::IsCollision(GLabel* other)
	{
		return true;
	}
}