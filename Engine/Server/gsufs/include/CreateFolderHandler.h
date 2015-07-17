#ifndef __AUGE_UFS_CREATE_FOLDER__HANDLER_H__
#define __AUGE_UFS_CREATE_FOLDER__HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class CreateFolderRequest;
	class FeatureClass;

	class CreateFolderHandler : public WebHandler
	{
	public:
		CreateFolderHandler();
		virtual ~CreateFolderHandler();
	public:
		virtual const char*		GetName();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);
	};
}

#endif //__AUGE_UFS_CREATE_FOLDER__HANDLER_H__
