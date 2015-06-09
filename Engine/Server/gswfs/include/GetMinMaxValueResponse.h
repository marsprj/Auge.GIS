#ifndef __AUGE_WFS_GET_MINMAX_VALUE_RESPONSE_H__
#define __AUGE_WFS_GET_MINMAX_VALUE_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class EnumValue;
	class GetValueRequest;
	class XElement;
	class GValue;

	class GetMinMaxValueResponse : public WebResponse
	{
	public:
		GetMinMaxValueResponse(GetValueRequest* pRequest);
		virtual ~GetMinMaxValueResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetValues(EnumValue* pValues);
		void	AddValue(XElement* pxParent, GValue* pValue);

	private:
		GetValueRequest*	m_pRequest;
		EnumValue*			m_pValues;
	};
}

#endif //__AUGE_WFS_GET_MINMAX_VALUE_RESPONSE_H__
