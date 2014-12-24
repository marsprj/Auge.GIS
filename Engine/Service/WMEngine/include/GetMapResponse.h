#ifndef __AUGE_GET_MAP_RESPONSE_H__
#define __AUGE_GET_MAP_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class GetMapRequest;

	class GetMapResponse : public WebResponse
	{
	public:
		GetMapResponse(GetMapRequest* pRequest);
		virtual ~GetMapResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetPath(const char* path);

	private:
		GetMapRequest*	m_pRequest;
		std::string		m_path;
	};
}

#endif //__AUGE_GET_MAP_RESPONSE_H__
