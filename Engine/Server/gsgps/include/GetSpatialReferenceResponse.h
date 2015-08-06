#ifndef __AUGE_GPS_GET_SPATIAL_REFERENCE_RESPONSE_H__
#define __AUGE_GPS_GET_SPATIAL_REFERENCE_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>
#include <sqlite3.h>

namespace auge
{
	class GetSpatialReferenceRequest;
	class WebContext;

	class GetSpatialReferenceResponse : public WebResponse
	{
	public:
		GetSpatialReferenceResponse(GetSpatialReferenceRequest* pRequest);
		virtual ~GetSpatialReferenceResponse();
	public:
		virtual	RESULTCODE		Write();
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void		SetWebContenxt(WebContext* pWebContext);
		void		SetSqlite(sqlite3_stmt* pstmt);
		XDocument*	WriteDocument();

	private:
		sqlite3_stmt* m_pstmt;
		WebContext	*m_pWebContext;
		GetSpatialReferenceRequest	*m_pRequest;
	};
}

#endif //__AUGE_GPS_GET_SPATIAL_REFERENCE_RESPONSE_H__
