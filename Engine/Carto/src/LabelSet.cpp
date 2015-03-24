#include "LabelSet.h"
#include "Label.h"

namespace auge
{
	LabelSet::LabelSet()
	{

	}

	LabelSet::~LabelSet()
	{
		Cleanup();
	}

	void LabelSet::Release()
	{
		delete this;
	}

	void LabelSet::Cleanup()
	{
		GLabel* pLabel = NULL;
		std::vector<GLabel*>::iterator iter;
		for(iter=m_labels.begin(); iter!=m_labels.end(); iter++)
		{
			pLabel = *iter;
			pLabel->Release();
		}
		m_labels.clear();
	}
}