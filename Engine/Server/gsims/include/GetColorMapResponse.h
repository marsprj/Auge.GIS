#ifndef __AUGE_IMS_GET_COLORMAP_RESPONSE_H__
#define __AUGE_IMS_GET_COLORMAP_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class ColorMap;
	class EnumColorMap;
	class GetColorMapRequest;
	class WebContext;

	class GetColorMapResponse : public WebResponse
	{
	public:
		GetColorMapResponse(GetColorMapRequest* pRequest);
		virtual ~GetColorMapResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetColorMaps(EnumColorMap* pColorMaps);
		void	SetColorMap(ColorMap* pColorMap);
		void	SetWebContext(WebContext* pWebContext);

	private:
		RESULTCODE	WriteColorMaps(WebWriter* pWriter);
		RESULTCODE	WriteColorMap(WebWriter* pWriter);

	private:
		GetColorMapRequest	*m_pRequest;
		EnumColorMap		*m_pColorMaps;
		ColorMap			*m_pColorMap;
		WebContext			*m_pWebContext;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__

