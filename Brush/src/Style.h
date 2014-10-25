#ifndef __AUGE_STYLE_H__
#define __AUGE_STYLE_H__

#include "Base.h"
#include <vector>

namespace auge
{
	class Rule;

	class AUGE_API Style : public Base
	{
	public:
		Style();
		virtual ~Style();

	public:
		std::vector<Rule*>& GetRules();
		uint				GetRuleCount();

		int					AddRule(Rule* pRule);

	private:
		void				CleanupRules();
		
	private:
		std::vector<Rule*>	m_rules;
	};

}//namespace

#endif //__AUGE_STYLE_H__