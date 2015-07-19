#ifndef __AUGE_GPS_CONVEXHULL_RESPONSE_H__
#define __AUGE_GPS_CONVEXHULL_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class ConvexHullRequest;
	class FeatureCursor;
	class WebContext;
	class GValue;

	class ConvexHullResponse : public WebResponse
	{
	public:
		ConvexHullResponse(ConvexHullRequest* pRequest);
		virtual ~ConvexHullResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void		SetWebContenxt(WebContext* pWebContext);
		void		SetFeatureCursor(FeatureCursor* pCursor);
		XDocument*	WriteDocument();

	private:
		void		WriteConvexHull(XElement* pxParent);

	private:
		ConvexHullRequest	*m_pRequest;
		WebContext			*m_pWebContext;
		FeatureCursor		*m_pCursor;

		std::string			m_encoding;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__

