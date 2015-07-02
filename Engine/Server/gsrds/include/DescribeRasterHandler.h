#ifndef __AUGE_RDS_DESCRIBE_RASTER__HANDLER_H__
#define __AUGE_RDS_DESCRIBE_RASTER__HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class DescribeRasterRequest;

	class DescribeRasterHandler : public WebHandler
	{
	public:
		DescribeRasterHandler();
		virtual ~DescribeRasterHandler();
	public:
		virtual const char*		GetName();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext);
	};
}

#endif //__AUGE_RDS_DESCRIBE_RASTER__HANDLER_H__
