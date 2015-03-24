#ifndef __AUGE_MAP_LABEL_H__
#define __AUGE_MAP_LABEL_H__

#include <vector>

namespace auge
{
	class LabelSet;
	class Renderer;

	class Maplex
	{
	public:
		Maplex();
		virtual ~Maplex();
	public:
		void		Draw(Renderer* pRenderer);

		LabelSet*	GetLabelSet(const char* name);
		void		Cleanup();

	private:
		std::vector<LabelSet*>	m_labelset;
	};
}

#endif //__AUGE_MAP_LABEL_H__
