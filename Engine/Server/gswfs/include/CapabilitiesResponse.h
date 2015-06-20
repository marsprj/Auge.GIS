#ifndef __AUGE_WFS_GETCAPABILITIES_RESPONSE_H__
#define __AUGE_WFS_GETCAPABILITIES_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class WFSCapabilitiesRequest;

	class WFSCapabilitiesResponse : public WebResponse
	{
	public:
		WFSCapabilitiesResponse(WFSCapabilitiesRequest* pRequest);
		virtual ~WFSCapabilitiesResponse();
	public:
		virtual	RESULTCODE		Write();
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetPath(const char* path);

	private:
		WFSCapabilitiesRequest*	m_pRequest;
		std::string		m_path;
	};
}

#endif //__AUGE_WFS_GETCAPABILITIES_RESPONSE_H__
