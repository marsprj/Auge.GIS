#ifndef __AUGE_IMS_GET_MAPSCALE_RESPONSE_H__
#define __AUGE_IMS_GET_MAPSCALE_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class ColorMap;
	class EnumColorMap;
	class GetMapScaleRequest;
	class WebContext;

	class GetMapScaleResponse : public WebResponse
	{
	public:
		GetMapScaleResponse(GetMapScaleRequest* pRequest);
		virtual ~GetMapScaleResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetMapScale(float mapScale);
		void	SetWebContext(WebContext* pWebContext);

	private:
		GetMapScaleRequest	*m_pRequest;
		float	m_map_scale;
		WebContext			*m_pWebContext;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__

