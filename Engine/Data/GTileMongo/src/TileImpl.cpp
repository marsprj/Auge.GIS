#include "TileImpl.h"

namespace auge
{
	TileImpl::TileImpl()
	{
		m_data = NULL;
		m_size = 0;
	}

	TileImpl::~TileImpl()
	{
		if(m_data)
		{
			free(m_data);
			m_data = NULL;
		}
	}

	const char* TileImpl::GetKey()
	{
		return m_key.c_str();
	}

	g_uchar* TileImpl::GetData()
	{
		return m_data;
	}

	g_uint TileImpl::GetSize()
	{
		return m_size;
	}

	void TileImpl::SetData(g_uchar* data)
	{
		if(m_data)
		{
			free(m_data);
		}
		m_data = data;
	}

	void TileImpl::SetSize(g_uint size)
	{
		m_size = size;
	}

	void TileImpl::SetKey(const char* key)
	{
		if(key)
		{
			m_key = key;
		}
		else
		{
			m_key.clear();
		}
	}
}