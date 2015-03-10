#ifndef __AUGE_WFS_GET_FEATURE_RESPONSE_H__
#define __AUGE_WFS_GET_FEATURE_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class GetFeatureRequest;
	class FeatureCursor;

	class GetFeatureResponse : public WebResponse
	{
	public:
		GetFeatureResponse(GetFeatureRequest* pRequest);
		virtual ~GetFeatureResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetFeatureCursor(FeatureCursor* pCursor);

	private:
		GetFeatureRequest	*m_pRequest;
		FeatureCursor		*m_pCursor;

		std::string			m_encoding;
	};
}

#endif //__AUGE_WFS_GET_FEATURE_RESPONSE_H__
