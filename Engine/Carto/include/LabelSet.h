#ifndef __AUGE_LABEL_SET_H__
#define __AUGE_LABEL_SET_H__

#include <vector>
#include <string>

namespace auge
{
	class GLabel;

	class LabelSet
	{
		friend class Maplex;

	public:
		LabelSet();
		virtual ~LabelSet();

	public:
		const char*	GetName();
		void		SetName(const char* name);
		void		Add(GLabel* pLabel);
		bool		IsCollision(GLabel* pLabel);
		void		Cleanup();
		void		Release();

	private:
		std::string m_name;
		std::vector<GLabel*>	m_labels;
	};
}

#endif //__AUGE_LABEL_SET_H__
