#ifndef __AUGE_LABEL_H__
#define __AUGE_LABEL_H__

#include "AugeObject.h"
#include "AugeType.h"
#include "AugeGeometry.h"
#include <string>

namespace auge
{
	class Geometry;
	class TextSymbolizer;
	class Renderer;
	class Transformation;

	class GLabel : public GObject
	{
	public:
		GLabel();
		virtual ~GLabel();

	public:
		virtual augeGeometryType GetType();
		virtual void			Draw(Renderer* pRenderer, Transformation* pTransformation);
		virtual bool			IsCollision(GLabel* other);
		virtual void			ComputePosition(Renderer* pRenderer, Transformation* pTransform);
		virtual void			AdjustPosition(int width, int height);

		virtual void			SetText(const char* text);
		virtual void			SetGeometry(Geometry* pGeometry);
		virtual void			SetSymbolizer(TextSymbolizer* pSymbolizer);

		virtual const char*		GetText();
		virtual const g_char*	GetTextW();
		virtual Geometry*		GetGeometry();
		virtual TextSymbolizer* GetSymbolizer();

		virtual void			Release();

	protected:
		std::string		m_text;
		std::wstring	m_wtext;
		Geometry		*m_pGeometry;
		TextSymbolizer	*m_pSymbolizer;
		augeGeometryType m_type;
	};
}

#endif //__AUGE_LABEL_H__
