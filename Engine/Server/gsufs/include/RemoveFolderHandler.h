#ifndef __AUGE_UFS_REMOVE_FOLDER__HANDLER_H__
#define __AUGE_UFS_REMOVE_FOLDER__HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class RemoveFolderRequest;
	class FeatureClass;

	class RemoveFolderHandler : public WebHandler
	{
	public:
		RemoveFolderHandler();
		virtual ~RemoveFolderHandler();
	public:
		virtual const char*		GetName();
		virtual const char*		GetDescription();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);
	};
}

#endif //__AUGE_UFS_REMOVE_FOLDER__HANDLER_H__
