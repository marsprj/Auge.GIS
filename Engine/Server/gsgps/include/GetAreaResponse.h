#ifndef __AUGE_GPS_GET_AREA_RESPONSE_H__
#define __AUGE_GPS_GET_AREA_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class GetAreaRequest;
	class FeatureCursor;
	class WebContext;
	class GValue;

	class GetAreaResponse : public WebResponse
	{
	public:
		GetAreaResponse(GetAreaRequest* pRequest);
		virtual ~GetAreaResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void		SetWebContenxt(WebContext* pWebContext);
		void		SetFeatureCursor(FeatureCursor* pCursor);
		XDocument*	WriteDocument();

	private:
		void		WriteGetArea(XElement* pxParent);

	private:
		GetAreaRequest	*m_pRequest;
		WebContext			*m_pWebContext;
		FeatureCursor		*m_pCursor;

		std::string			m_encoding;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__

