#ifndef __AUGE_IMS_GET_PREVIEWRESPONSE_H__
#define __AUGE_IMS_GET_PREVIEWRESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class GetPreviewRequest;

	class GetPreviewResponse : public WebResponse
	{
	public:
		GetPreviewResponse(GetPreviewRequest* pRequest);
		virtual ~GetPreviewResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetPath(const char* path);

	private:
		GetPreviewRequest*	m_pRequest;
		std::string		m_path;
	};
}

#endif //__AUGE_IMS_GET_PREVIEWRESPONSE_H__
