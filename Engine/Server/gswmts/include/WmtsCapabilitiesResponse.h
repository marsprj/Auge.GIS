#ifndef __AUGE_WMTS_CAPABILITIES_RESPONSE_H__
#define __AUGE_WMTS_CAPABILITIES_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class WmtsCapabilitiesRequest;

	class WmtsCapabilitiesResponse : public WebResponse
	{
	public:
		WmtsCapabilitiesResponse(WmtsCapabilitiesRequest* pRequest);
		virtual ~WmtsCapabilitiesResponse();

	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetPath(const char* path);

	private:
		WmtsCapabilitiesRequest*	m_pRequest;
		std::string				m_path;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__

