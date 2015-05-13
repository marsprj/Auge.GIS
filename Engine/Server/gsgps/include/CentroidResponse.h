#ifndef __AUGE_GPS_CENTROID_RESPONSE_H__
#define __AUGE_GPS_CENTROID_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class CentroidRequest;
	class FeatureCursor;
	class WebContext;

	class CentroidResponse : public WebResponse
	{
	public:
		CentroidResponse(CentroidRequest* pRequest);
		virtual ~CentroidResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void		SetWebContenxt(WebContext* pWebContext);
		void		SetFeatureCursor(FeatureCursor* pCursor);
		XDocument*	WriteDocument();

	private:
		CentroidRequest*	m_pRequest;
		WebContext			*m_pWebContext;
		FeatureCursor		*m_pCursor;

		std::string			m_encoding;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__

