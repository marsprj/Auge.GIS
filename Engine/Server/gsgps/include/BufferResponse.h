#ifndef __AUGE_GPS_BUFFER_RESPONSE_H__
#define __AUGE_GPS_BUFFER_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class BufferRequest;
	class FeatureCursor;
	class WebContext;
	class GValue;

	class BufferResponse : public WebResponse
	{
	public:
		BufferResponse(BufferRequest* pRequest);
		virtual ~BufferResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void		SetWebContenxt(WebContext* pWebContext);
		void		SetFeatureCursor(FeatureCursor* pCursor);
		XDocument*	WriteDocument();

	private:
		void		WriteBuffer(const char* distance_field, XElement* pxParent);
		void		WriteBuffer(double distance, XElement* pxParent);
		double		GetDistance(GValue* pValue);


	private:
		BufferRequest		*m_pRequest;
		WebContext			*m_pWebContext;
		FeatureCursor		*m_pCursor;

		std::string			m_encoding;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__

