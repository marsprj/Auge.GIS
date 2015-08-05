#ifndef __AUGE_RDS_LIST_RESPONSE_H__
#define __AUGE_RDS_LIST_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <vector>
#include <string>

#ifdef WIN32
#	include <io.h>
#include <sys\stat.h>
#endif

namespace auge
{
	class RasterFolder;
	class EnumRaster;
	class EnumRasterFolder;

	class ListResponse : public WebResponse
	{
	public:
		ListResponse(ListRequest* pRequest);
		virtual ~ListResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	
	public:
		void		SetFolder(RasterFolder* pFolder);

	private:
		XDocument*	WriteDocument();
		void		WriteRasters(XElement* pxFiles, EnumRaster* pRasters);
		void		WriteFolders(XElement* pxFiles, EnumRasterFolder* pFolders);
		

	private:
		ListRequest		*m_pRequest;
		RasterFolder	*m_pFolder;
	};
}

#endif //__AUGE_RDS_LIST_RESPONSE_H__
