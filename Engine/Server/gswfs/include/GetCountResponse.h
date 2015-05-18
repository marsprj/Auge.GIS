#ifndef __AUGE_WFS_GET_COUNT_RESPONSE_H__
#define __AUGE_WFS_GET_COUNT_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class GetCountRequest;
	class FeatureCursor;
	class WebContext;

	class GetCountResponse : public WebResponse
	{
	public:
		GetCountResponse(GetCountRequest* pRequest);
		virtual ~GetCountResponse();
	public:
		virtual	RESULTCODE		Write();
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void		SetWebContenxt(WebContext* pWebContext);
		void		SetCount(g_uint count);
		XDocument*	WriteDocument();

	private:
		GetCountRequest	*m_pRequest;
		WebContext		*m_pWebContext;
		g_uint			m_count;

		std::string		m_encoding;
	};
}

#endif //__AUGE_WFS_GET_COUNT_RESPONSE_H__
