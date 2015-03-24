#ifndef __AUGE_LABEL_SET_H__
#define __AUGE_LABEL_SET_H__

#include <vector>

namespace auge
{
	class GLabel;

	class LabelSet
	{
	public:
		LabelSet();
		virtual ~LabelSet();

	public:
		void		Cleanup();
		void		Release();

	private:
		std::vector<GLabel*>	m_labels;
	};
}

#endif //__AUGE_LABEL_SET_H__
