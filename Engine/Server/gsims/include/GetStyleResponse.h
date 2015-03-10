#ifndef __AUGE_IMS_GET_STYLE_RESPONSE_H__
#define __AUGE_IMS_GET_STYLE_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class Service;
	class GetStyleRequest;
	class EnumStyle;

	class GetStyleResponse : public WebResponse
	{
	public:
		GetStyleResponse(GetStyleRequest* pRequest);
		virtual ~GetStyleResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
		
	public:
		void	SetStyles(EnumStyle* pStyles);
		void	SetStyleText(char* text);
		
	private:
		GetStyleRequest *m_pRequest;	
		EnumStyle		*m_pStyles;
		char			*m_style_text;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
