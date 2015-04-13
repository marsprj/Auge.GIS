#ifndef __AUGE_IMS_GET_FONT_RESPONSE_H__
#define __AUGE_IMS_GET_FONT_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class Service;
	class Workspace;
	class GetFontRequest;

	class GetFontResponse : public WebResponse
	{
	public:
		GetFontResponse(GetFontRequest* pRequest);
		virtual ~GetFontResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	public:
		void	SetPath(const char* path);

	private:
		std::string m_path;
		GetFontRequest *m_pRequest;	
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
