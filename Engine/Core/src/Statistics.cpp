#include "AugeCore.h"

namespace auge
{
	GStatistics::GStatistics()
	{
	}

	GStatistics::~GStatistics()
	{
		
	}

	const char* GStatistics::GetKey(g_uint i)
	{
		if(i>=m_items.size())
		{
			return NULL;
		}
		return m_items[i].item;
	}

	g_uint GStatistics::GetValue(g_uint i)
	{
		if(i>=m_items.size())
		{
			return 0;
		}
		return m_items[i].count;
	}

	g_uint GStatistics::GetCount()
	{
		return m_items.size();
	}

	bool GStatistics::Add(const char* key, g_uint count)
	{
		if(key==NULL)
		{
			return false;
		}

		return true;
	}

	void GStatistics::SetCount(g_uint count)
	{
		m_items.resize(count);
	}

	bool GStatistics::SetValue(g_uint i, const char* key, g_uint count)
	{
		if(i>=m_items.size())
		{
			return false;
		}
		if(key==NULL)
		{
			return false;
		}
		memset(m_items[i].item, 0, NAME_SHORT);
		strcpy(m_items[i].item, key);
		m_items[i].count = count;
		return true;
	}

	void GStatistics::Release()
	{
		delete this;
	}
}