#include "Maplex.h"
#include "LabelSet.h"

namespace auge
{
	Maplex::Maplex()
	{

	}

	Maplex::~Maplex()
	{
		Cleanup();
	}

	void Maplex::Draw(Renderer* pRenderer)
	{

	}

	LabelSet* Maplex::GetLabelSet(const char* name)
	{
		return NULL;
	}

	void Maplex::Cleanup()
	{
		LabelSet* pLabelSet = NULL;
		std::vector<LabelSet*>::iterator iter;
		for(iter=m_labelset.begin(); iter!=m_labelset.end(); iter++)
		{
			pLabelSet = *iter;
			pLabelSet->Release();
		}
		m_labelset.clear();
	}
}