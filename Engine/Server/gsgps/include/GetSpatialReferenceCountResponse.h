#ifndef __AUGE_GPS_GET_SPATIAL_REFERENCE_COUNT_RESPONSE_H__
#define __AUGE_GPS_GET_SPATIAL_REFERENCE_COUNT_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>
#include <sqlite3.h>

namespace auge
{
	class GetSpatialReferenceCountRequest;
	class WebContext;

	class GetSpatialReferenceCountResponse : public WebResponse
	{
	public:
		GetSpatialReferenceCountResponse(GetSpatialReferenceCountRequest* pRequest);
		virtual ~GetSpatialReferenceCountResponse();
	public:
		virtual	RESULTCODE		Write();
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void		SetWebContenxt(WebContext* pWebContext);
		void		SetSqlite(sqlite3_stmt* pstmt);
		XDocument*	WriteDocument();

	private:
		WebContext	*m_pWebContext;
		GetSpatialReferenceCountRequest	*m_pRequest;
	};
}

#endif //__AUGE_GPS_GET_SPATIAL_REFERENCE_COUNT_RESPONSE_H__
