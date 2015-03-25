#include "Maplex.h"
#include "LabelSet.h"
#include "Label.h"
#include "PointLabel.h"
#include "LineLabel.h"
#include "PolygonLabel.h"

namespace auge
{
	Maplex::Maplex():
	m_pRenderer(NULL),
	m_pTransformation(NULL)
	{

	}

	Maplex::~Maplex()
	{
		Cleanup();
	}

	void Maplex::SetExtent(int width, int height)
	{
		m_width = width;
		m_height = height;
	}

	void Maplex::Draw()
	{
		LabelSet* pLabelSet = NULL;
		std::vector<LabelSet*>::iterator iter;
		for(iter=m_labelset.begin(); iter!=m_labelset.end(); iter++)
		{
			pLabelSet = *iter;
			DrawLabelSet(pLabelSet);
		}
	}

	LabelSet* Maplex::GetLabelSet(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}
		LabelSet* pLabelSet = NULL;
		std::vector<LabelSet*>::iterator iter;
		for(iter=m_labelset.begin(); iter!=m_labelset.end(); iter++)
		{
			pLabelSet = *iter;
			if(!strcmp(pLabelSet->GetName(), name))
			{
				return pLabelSet;
			}
		}
		if(pLabelSet==NULL)
		{
			pLabelSet = new LabelSet();
			pLabelSet->SetName(name);
			m_labelset.push_back(pLabelSet);
		}
		return pLabelSet;
	}

	bool Maplex::AddLabel(const char* set_name, GLabel* pLabel)
	{
		LabelSet* pSet = GetLabelSet(set_name);
		if(pSet==NULL)
		{
			return false;
		}

		std::vector<LabelSet*>::iterator iter;
		for(iter=m_labelset.begin(); iter!=m_labelset.end(); iter++)
		{
			if((*iter)->IsCollision(pLabel))
			{
				return false;
			}
		}

		pSet->Add(pLabel);
		return true;
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

	void Maplex::SetRenderer(Renderer* pRenderer)
	{
		m_pRenderer = pRenderer;
	}

	void Maplex::SetTransformation(Transformation* pTransformation)
	{
		m_pTransformation = pTransformation;
	}

	void Maplex::DrawLabelSet(LabelSet* pLabelSet)
	{	
		std::vector<GLabel*>& labels = pLabelSet->m_labels;
		std::vector<GLabel*>::iterator iter=labels.begin();

		GLabel* pLabel = *iter;
		TextSymbolizer* pSymbolizer = pLabel->GetSymbolizer();
		for(; iter!=labels.end(); iter++)
		{
			pLabel = *iter;
			pLabel->Draw(m_pRenderer, m_pTransformation);
		}
	}

	bool Maplex::IsCollision(GLabel* label)
	{
		if(label==NULL)
		{
			return false;
		}

		LabelSet* pSet = NULL;
		std::vector<LabelSet*>::iterator iter;
		for(iter=m_labelset.begin(); iter!=m_labelset.end(); iter++)
		{
			if((*iter)->IsCollision(label))
			{
				return true;
			}
		}

		return false;
	}
}