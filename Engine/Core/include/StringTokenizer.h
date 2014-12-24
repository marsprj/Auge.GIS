#ifndef __AUGE_STRING_TOKENIZE_H__
#define __AUGE_STRING_TOKENIZE_H__

#include <string>
#include <vector>

namespace auge
{

	class StringTokenizer
	{
	public:
		StringTokenizer(const char* wkt, char delim);
		virtual ~StringTokenizer();

		int         GetCount() const;
		const char* GetItem(int n) const;	
		void        Remove(std::string l, std::string t);

	public:
		static void Remove(std::string& s, std::string l, std::string t);
	private:
		std::vector<std::string> m_tokens; 
	};

}

#endif //__AUGE_STRING_TOKENIZE_H__
