#include <StringTokenizer.h>

namespace auge
{
	StringTokenizer::StringTokenizer(const char* wkt, char delim) 
	{
		std::string data = wkt;

		m_tokens.clear();

		int nest = 0, s, p;
		int iSize = data.size();
		for (p = 0, s = 0; p < iSize; p++) {
			char c = data[p];
			// increase nesting if an open character is found
			if (c == '(' || c == '[' || c == '<') {
				nest++;
			}
			// decrease nesting if a close character is found
			if (c == ')' || c == ']' || c == '>') {
				nest--;
			}
			if (nest == 0 && c == delim) 
			{
				//m_tokens.push_back(data.substr(s, p));
				/* Renyc 2010-04-08 15:01 */
				m_tokens.push_back(data.substr(s, p-s));
				s = p + 1; // +1 to skip the delimiter
			}
		}
		// Don't forget the last token ;-)
		if ((unsigned)s < data.length()) {
			m_tokens.push_back(data.substr(s));
		}
	}	

	StringTokenizer::~StringTokenizer()
	{

	}

	int StringTokenizer::GetCount()const
	{
		return m_tokens.size();
	}

	const char* StringTokenizer::GetItem(int n) const
	{
		return m_tokens[n].c_str();
	}

	void StringTokenizer::Remove(std::string& s, std::string l, std::string t) 
	{
		int npos = s.find(l);
		if(npos>=0)
		{
			s = s.substr(npos+1, s.length()-npos-1);
			npos = s.find_last_of(t);
			s = s.substr(0, npos);
		}
		// 	if (s.find(l)==0) {
		// 		s = s.substr(l.size());
		// 	}
		// 	if (s.find_last_of(t)==(s.size()-t.size())) {
		// 		s = s.substr(0, s.size()-t.size());
		// 	}
	}

	void StringTokenizer::Remove(std::string l, std::string t)
	{
		for (int i = 0; i < m_tokens.size(); i++) {
			Remove(m_tokens[i], l, t);
		}
	}
}
