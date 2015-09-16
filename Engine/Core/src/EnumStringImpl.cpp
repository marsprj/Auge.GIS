#include "AugeCore.h"

namespace auge
{
	EnumString::EnumString()
	{
		m_iter = m_strs.begin();
	}

	EnumString::~EnumString()
	{
		Cleanup();
	}

	void EnumString::Reset()
	{
		m_iter = m_strs.begin();
	}

	const char*	EnumString::Next()
	{
		if(m_iter==m_strs.end())
		{
			return NULL;
		}
		const char* str = m_iter->c_str();
		m_iter++;
		return str;
	}

	void EnumString::Release()
	{
		delete this;
	}

	void EnumString::Add(const char* str)
	{
		if(str==NULL)
		{
			return;
		}
		m_strs.push_back(str);
	}

	void EnumString::Cleanup()
	{
		m_strs.clear();
	}
}