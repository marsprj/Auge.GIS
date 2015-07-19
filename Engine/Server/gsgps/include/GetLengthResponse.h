#ifndef __AUGE_GPS_GET_LENGTH_RESPONSE_H__
#define __AUGE_GPS_GET_LENGTH_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class GetLengthRequest;
	class FeatureCursor;
	class WebContext;
	class GValue;

	class GetLengthResponse : public WebResponse
	{
	public:
		GetLengthResponse(GetLengthRequest* pRequest);
		virtual ~GetLengthResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void		SetWebContenxt(WebContext* pWebContext);
		void		SetFeatureCursor(FeatureCursor* pCursor);
		XDocument*	WriteDocument();

	private:
		void		WriteGetLength(XElement* pxParent);

	private:
		GetLengthRequest	*m_pRequest;
		WebContext			*m_pWebContext;
		FeatureCursor		*m_pCursor;

		std::string			m_encoding;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__

