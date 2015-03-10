#ifndef __AUGE_WFS_TRANSACTION_HANDLER_H__
#define __AUGE_WFS_TRANSACTION_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class Map;
	class WebContext;
	class TransactionRequest;
	class TransactionResponse;
	class FeatureClass;

	class TransactionHandler : public WebHandler
	{
	public:
		TransactionHandler();
		virtual ~TransactionHandler();
	public:
		virtual const char*		GetName();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc);
		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap);
	};
}

#endif //__AUGE_WFS_TRANSACTION_HANDLER_H__

