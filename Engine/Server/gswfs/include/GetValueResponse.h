#ifndef __AUGE_WFS_GETVALUE_RESPONSE_H__
#define __AUGE_WFS_GETVALUE_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class EnumValue;
	class GetValueRequest;

	class GetValueResponse : public WebResponse
	{
	public:
		GetValueResponse(GetValueRequest* pRequest);
		virtual ~GetValueResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetValues(EnumValue* pValues);
		
	private:
		GetValueRequest*	m_pRequest;
		EnumValue*			m_pValues;
	};
}

#endif //__AUGE_WFS_GETVALUE_RESPONSE_H__
