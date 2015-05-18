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
		return NULL;
	}

	g_uchar* TileImpl::GetData()
	{
		if(m_data==NULL)
		{
			if(g_access(m_path.c_str(),4))
			{
				return NULL;
			}

			FILE*fp = fopen(m_path.c_str(), "rb");
			if (fp == NULL)
			{
				return false;
			}

			fseek(fp, 0, SEEK_SET);
			fseek(fp, 0, SEEK_END);

			m_size = ftell(fp);
			m_data = (g_uchar*)malloc(m_size);
			memset(m_data, 0, m_size);

			fseek(fp, 0, SEEK_SET);
			size_t nread = fread(m_data, sizeof(g_uchar), m_size, fp);
			if (nread < m_size)
			{
				free(m_data);
				m_data = NULL;
				m_size = 0;
			}
			fclose(fp);
		}
		return m_data;
	}

	g_uint TileImpl::GetSize()
	{
		if(!m_size)
		{
			GetData();
		}
		return m_size;
	}

	bool TileImpl::Create(const char* path)
	{
		if(path==NULL)
		{
			return false;
		}
		m_path = path;
		return true;
	}
}