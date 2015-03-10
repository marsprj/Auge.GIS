#ifndef __AUGE_WFS_GET_FEATURE_RESPONSE_H__
#define __AUGE_WFS_GET_FEATURE_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class GetGmlObjectRequest;
	class FeatureCursor;

	class GetGmlObjectResponse : public WebResponse
	{
	public:
		GetGmlObjectResponse(GetGmlObjectRequest* pRequest);
		virtual ~GetGmlObjectResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetFeatureCursor(FeatureCursor* pCursor);

	private:
		GetGmlObjectRequest	*m_pRequest;
		FeatureCursor		*m_pCursor;

		std::string			m_encoding;
	};
}

#endif //__AUGE_WFS_GET_FEATURE_RESPONSE_H__
