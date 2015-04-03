#include "AugeCore.h"

namespace auge
{
	EnumValue::EnumValue()
	{
		m_iter = m_values.begin();
	}

	EnumValue::~EnumValue()
	{
		Cleanup();
	}

	void EnumValue::Reset()
	{
		m_iter = m_values.begin();
	}

	GValue*	EnumValue::Next()
	{
		if(m_iter==m_values.end())
		{
			return NULL;
		}
		return *(m_iter++);
	}

	void EnumValue::Release()
	{
		delete this;
	}

	void EnumValue::Add(GValue* pValue)
	{
		m_values.push_back(pValue);
	}

	void EnumValue::Cleanup()
	{
		std::vector<GValue*>::iterator iter;
		for(iter=m_values.begin(); iter!=m_values.end(); iter++)
		{
			(*iter)->Release();
		}
		m_values.clear();
	}
}