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

	void LabelSet::Add(GLabel* pLabel)
	{
		if(pLabel!=NULL)
		{
			m_labels.push_back(pLabel);
		}
	}

	void LabelSet::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		else
		{
			m_name = name;
		}
	}

	const char* LabelSet::GetName()
	{
		return m_name.c_str();
	}

	bool LabelSet::IsCollision(GLabel* pLabel)
	{
		if(pLabel==NULL)
		{
			return false;
		}

		GLabel* label = NULL;
		std::vector<GLabel*>::iterator iter;
		for(iter=m_labels.begin(); iter!=m_labels.end(); iter++)
		{
			label = *iter;
			if(label->IsCollision(pLabel))
			{
				return true;
			}
		}

		return false;
	}
}