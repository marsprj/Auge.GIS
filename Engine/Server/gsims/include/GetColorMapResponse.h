#ifndef __AUGE_IMS_GET_COLORMAP_RESPONSE_H__
#define __AUGE_IMS_GET_COLORMAP_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class EnumColorMap;
	class GetColorMapRequest;

	class GetColorMapResponse : public WebResponse
	{
	public:
		GetColorMapResponse(GetColorMapRequest* pRequest);
		virtual ~GetColorMapResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetColorMaps(EnumColorMap* pColorMaps);

	private:
		GetColorMapRequest	*m_pRequest;
		EnumColorMap		*m_pColorMaps;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__

