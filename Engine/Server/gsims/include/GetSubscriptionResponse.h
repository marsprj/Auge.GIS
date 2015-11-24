#ifndef __AUGE_GET_SUBSCRIPTION_RESPONSE_H__
#define __AUGE_GET_SUBSCRIPTION_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class Service;
	class GetSubscriptionRequest;
	class SubscriptionTheme;
	class EnumSubscriptionTheme;

	class GetSubscriptionResponse : public WebResponse
	{
	public:
		GetSubscriptionResponse(GetSubscriptionRequest* pRequest);
		virtual ~GetSubscriptionResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void		SetTheme(SubscriptionTheme* pTheme);
		void		SetThemes(EnumSubscriptionTheme* pThemes);
		void		SetWebContext(WebContext* pWebContext);

		RESULTCODE	Write(WebWriter* pWriter, SubscriptionTheme* pTheme);
		RESULTCODE	Write(WebWriter* pWriter, EnumSubscriptionTheme* pThemes);

	private:
	private:
		SubscriptionTheme* m_pTheme;
		EnumSubscriptionTheme* m_pThemes;
		GetSubscriptionRequest *m_pRequest;
		WebContext* m_pWebContext;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
