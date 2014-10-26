#include "Style.h"
#include "Rule.h"

namespace auge
{

	Style::Style()
	{

	}

	Style::~Style()
	{
		CleanupRules();
	}

	const char* Style::GetName()
	{
		return m_name.c_str();
	}

	int	Style::SetName(const char* szName)
	{
		if(szName==NULL)
		{
			return AG_FAILURE;
		}
		m_name = szName;
		return AG_SUCCESS;
	}

	std::vector<Rule*>& Style::GetRules()
	{
		return m_rules;
	}

	uint Style::GetRuleCount()
	{
		return m_rules.size();
	}

	void Style::CleanupRules()
	{
		Rule *r = NULL;
		std::vector<Rule*>::iterator iter;
		for(iter=m_rules.begin(); iter!=m_rules.end(); iter++)
		{
			r = *iter;
			r->Release();
		}
		m_rules.clear();
	}

	int Style::AddRule(Rule* pRule)
	{
		if(pRule==NULL)
		{
			return AG_FAILURE;
		}
		m_rules.push_back(pRule);
		return AG_SUCCESS;
	}
	
}