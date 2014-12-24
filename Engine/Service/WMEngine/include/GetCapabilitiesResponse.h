#ifndef __AUGE_WMS_GETCAPABILITIES_RESPONSE_H__
#define __AUGE_WMS_GETCAPABILITIES_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class GetCapabilitiesRequest;

	class GetCapabilitiesResponse : public WebResponse
	{
	public:
		GetCapabilitiesResponse(GetCapabilitiesRequest* pRequest);
		virtual ~GetCapabilitiesResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetPath(const char* path);

	private:
		GetCapabilitiesRequest*	m_pRequest;
		std::string		m_path;
	};
}

#endif //__AUGE_WMS_GETCAPABILITIES_RESPONSE_H__
