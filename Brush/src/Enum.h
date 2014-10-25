#ifndef __AUGE_ENUM_H__
#define __AUGE_ENUM_H__

#include <vector>

namespace auge
{
	template <class T>
	class Enum
	{
	public:
		Enum(void);
		~Enum(void);

	private:
		std::vector<T>	m_elemets;
	};
}

#endif //__AUGE_ENUM_H__
