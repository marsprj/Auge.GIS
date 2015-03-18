#include "FeatureStyleImpl.h"

namespace auge{

	FeatureStyleImpl::FeatureStyleImpl()
	{

	}

	FeatureStyleImpl::~FeatureStyleImpl()
	{
		Clear();
	}

	//////////////////////////////////////////////////////////////////////////

	const char*	FeatureStyleImpl::GetName()
	{
		return m_name.c_str();
	}

	void FeatureStyleImpl::SetName(const char* szName)
	{
		if(szName==NULL)
		{
			m_name.clear();
		}
		m_name = szName;
	}

	augeStyleType FeatureStyleImpl::GetType()
	{
		return augeStyleFeature;
	}

	augeGeometryType FeatureStyleImpl::GetGeometryType()
	{
		return m_geom_type;
	}

	void FeatureStyleImpl::SetGeometryType(augeGeometryType type)
	{
		m_geom_type = type;
	}

	const char*	FeatureStyleImpl::GetTitle()
	{
		return NULL;
	}

	void FeatureStyleImpl::SetTitle(const char* szTitle)
	{

	}

	const char*	FeatureStyleImpl::GetAbstract()
	{
		return NULL;
	}

	void FeatureStyleImpl::SetAbstract(const char* szAbstract)
	{

	}

	const char* FeatureStyleImpl::GetFeatureTypeName()
	{
		return NULL;
	}

	bool FeatureStyleImpl::SetFeatureTypeName(const char* szName)
	{
		return true;
	}

	int	FeatureStyleImpl::GetRuleCount()
	{
		return m_rules.size();
	}

	Rule*	FeatureStyleImpl::GetRule(int i)
	{
		if(i<0||i>=m_rules.size())
			return NULL;
		return m_rules[i];
	}

	bool FeatureStyleImpl::AddRule(Rule* pRule)
	{
		if(pRule==NULL)
			return false;
		m_rules.push_back(pRule);
		return true;
	}

	void FeatureStyleImpl::Release()
	{
		delete this;
	}

	void FeatureStyleImpl::Clear()
	{
		Rule* pRule = NULL;
		RULE_VECTOR::iterator iter;
		for(iter=m_rules.begin(); iter!=m_rules.end(); iter++)
		{
			pRule = (*iter);
			if(pRule!=NULL)
			{
				pRule->Release();
			}
		}
		m_rules.clear();
	}

}
