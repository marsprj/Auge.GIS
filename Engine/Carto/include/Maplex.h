#ifndef __AUGE_MAP_LABEL_H__
#define __AUGE_MAP_LABEL_H__

#include <vector>
#include "LabelSet.h"

namespace auge
{
	class GLabel;
	class LabelSet;
	class Renderer;
	class Transformation;

	class Maplex
	{
	public:
		Maplex();
		virtual ~Maplex();
	public:
		void		Draw();
		void		SetExtent(int width, int height);

		LabelSet*	GetLabelSet(const char* name);
		bool		AddLabel(const char* set_name, GLabel* pLabel);
		bool		IsCollision(GLabel* label);
		void		Cleanup();

		void		SetRenderer(Renderer* m_pRenderer);
		void		SetTransformation(Transformation* pTransformation);

	private:
		void		DrawLabelSet(LabelSet* pLabelSet);

	private:
		std::vector<LabelSet*>	m_labelset;
		LabelSet	m_labeled;
		Renderer	*m_pRenderer;
		Transformation *m_pTransformation;
		int			m_width;
		int			m_height;

	};
}

#endif //__AUGE_MAP_LABEL_H__
