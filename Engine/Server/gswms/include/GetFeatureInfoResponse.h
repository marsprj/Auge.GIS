#ifndef __AUGE_GET_MAP_RESPONSE_H__
#define __AUGE_GET_MAP_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class GetFeatureInfoRequest;

	class GetFeatureInfoResponse : public WebResponse
	{
	public:
		GetFeatureInfoResponse(GetFeatureInfoRequest* pRequest);
		virtual ~GetFeatureInfoResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetPath(const char* path);

	private:
		GetFeatureInfoRequest*	m_pRequest;
		std::string		m_path;
	};
}

#endif //__AUGE_GET_MAP_RESPONSE_H__
