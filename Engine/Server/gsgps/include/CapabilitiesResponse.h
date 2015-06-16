#ifndef __AUGE_GPS_CAPABILITIES_RESPONSE_H__
#define __AUGE_GPS_CAPABILITIES_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class ProcessorCapabilitiesRequest;

	class ProcessorCapabilitiesResponse : public WebResponse
	{
	public:
		ProcessorCapabilitiesResponse(ProcessorCapabilitiesRequest* pRequest);
		virtual ~ProcessorCapabilitiesResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetPath(const char* path);

	private:
		ProcessorCapabilitiesRequest*	m_pRequest;
		std::string				m_path;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__

