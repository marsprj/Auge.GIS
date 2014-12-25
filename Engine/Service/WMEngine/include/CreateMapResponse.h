#ifndef __AUGE_WMS_CREATEMAP_RESPONSE_H__
#define __AUGE_WMS_CREATEMAP_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class CreateMapRequest;

	class CreateMapResponse : public WebResponse
	{
	public:
		CreateMapResponse(CreateMapRequest* pRequest);
		virtual ~CreateMapResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetPath(const char* path);

	private:
		CreateMapRequest*	m_pRequest;
		std::string		m_path;
	};
}

#endif //__AUGE_WMS_CREATEMAP_RESPONSE_H__
