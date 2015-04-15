#ifndef __AUGE_IMS_GET_PREVIEW_REQUEST_H__
#define __AUGE_IMS_GET_PREVIEW_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GetPreviewRequest : public WebRequest 
	{
	public:
		GetPreviewRequest();
		virtual ~GetPreviewRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetFormat();

		virtual const char*		GetSourceName();
		virtual const char*		GetDataSetName();
		virtual g_int			GetWidth();
		virtual g_int			GetHeight();
		virtual GEnvelope&		GetExtent();

	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* value);
		void		SetSourceName(const char* name);
		void		SetDataSetName(const char* name);
		void		SetWidth(const char* width);
		void		SetHeight(const char* height);
		void		SetExtent(const char* extent); 

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_source_name;
		std::string m_dataset_name;
		std::string m_format;
		g_int		m_width;
		g_int		m_height;
		GEnvelope	m_extent;
	};
}

#endif //__AUGE_IMS_GET_PREVIEW_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=ims&version=1.0.0&request=GetPreview&sourceName=db1&dataSetName=cities&width=800&height=600
http://127.0.0.1:8088/ows/user1/mgr?service=ims&version=1.0.0&request=GetPreview&sourceName=db1&dataSetName=cities&width=800&height=600

************************************************************************/