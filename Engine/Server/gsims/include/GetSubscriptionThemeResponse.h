#ifndef __AUGE_GET_SUBSCRIPTION_THEME_RESPONSE_H__
#define __AUGE_GET_SUBSCRIPTION_THEME_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class Service;
	class GetSubscriptionThemeRequest;
	class SubscriptionTheme;
	class EnumSubscriptionTheme;

	class GetSubscriptionThemeResponse : public WebResponse
	{
	public:
		GetSubscriptionThemeResponse(GetSubscriptionThemeRequest* pRequest);
		virtual ~GetSubscriptionThemeResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void		SetWebContext(WebContext* pWebContext);

		RESULTCODE	Write(WebWriter* pWriter, SubscriptionTheme* pTheme);
		RESULTCODE	Write(WebWriter* pWriter, EnumSubscriptionTheme* pThemes);

	private:
		GetSubscriptionThemeRequest *m_pRequest;
		WebContext* m_pWebContext;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
