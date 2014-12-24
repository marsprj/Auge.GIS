#ifndef __AUGE_ERROR_IMPL_H__
#define __AUGE_ERROR_IMPL_H__

#include "AugeCore.h"

namespace auge
{
	class GErrorImpl : public GError
	{
	public:
		GErrorImpl();
		virtual ~GErrorImpl();
	public:
		virtual void		SetError(const char* szError);
		virtual const char*	GetLastError();

	private:
		char m_error[AUGE_MSG_MAX];
	};

}

#endif // __AUGE_ERROR_IMPL_H