#ifndef __AUGE_POI_GET_POI_RESPONSE_H__
#define __AUGE_POI_GET_POI_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <vector>
#include <string>


namespace auge
{	
	class GResultSet;

	class GetPoiResponse : public WebResponse
	{
	public:
		GetPoiResponse(GetPoiRequest* pRequest);
		virtual ~GetPoiResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	
	public:
		void	AddFile(const char* name);
		void	SetResultset(GResultSet* pResultset);

	private:
		XDocument*			WriteDocument();

	private:
		GetPoiRequest		*m_pRequest;
		GResultSet			*m_pResultset;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
