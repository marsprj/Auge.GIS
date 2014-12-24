#include "GErrorImpl.h"
#include <stdio.h>
#include <stdlib.h>

namespace auge
{
	GError*	augeGetErrorInstance()
	{
		static GErrorImpl g_error;
		return &g_error;
	}

	GErrorImpl::GErrorImpl()
	{
		memset(m_error, 0, AUGE_NAME_MAX);
	}

	GErrorImpl::~GErrorImpl()
	{

	}

	void GErrorImpl::SetError(const char* szError)
	{
		if(szError==NULL)
		{
			m_error[0] = '\0';
		}
		g_sprintf(m_error, "%s", szError);
	}

	const char*	GErrorImpl::GetLastError()
	{
		return m_error;
	}

}
