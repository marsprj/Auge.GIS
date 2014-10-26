#ifndef __AUGE_STYLE_H__
#define __AUGE_STYLE_H__

#include "Base.h"
#include <vector>
#include <string>

namespace auge
{
	class Rule;

	class AUGE_API Style : public Base
	{
	public:
		Style();
		virtual ~Style();

	public:
		const char*			GetName();
		int					SetName(const char* szName);

		std::vector<Rule*>& GetRules();
		uint				GetRuleCount();

		int					AddRule(Rule* pRule);

	private:
		void				CleanupRules();
		
	private:
		std::string			m_name;
		std::vector<Rule*>	m_rules;
	};

}//namespace

#endif //__AUGE_STYLE_H__