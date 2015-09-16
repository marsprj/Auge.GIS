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
	class XNodeSet;
	class XNode;
	class GField;
	class FeatureWorkspace;

	class TransactionHandler : public WebHandler
	{
	public:
		TransactionHandler();
		virtual ~TransactionHandler();
	public:
		virtual const char*		GetName();
		virtual const char*		GetDescription();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		//virtual WebRequest*		ParseRequest(rude::CGI& cgi, WebContext* pWebContext=NULL, Map* pMap=NULL);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);
		//virtual WebRequest*		ParseRequest(XDocument* pxDoc, WebContext* pWebContext=NULL, Map* pMap=NULL);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);
		//virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap);

	private:
		g_uint			Insert(XNodeSet* pxNodeSet, WebContext* pWebContext, Map* pMap);
		g_uint			Update(XNodeSet* pxNodeSet, WebContext* pWebContext, Map* pMap);
		g_uint			Delete(XNodeSet* pxNodeSet, WebContext* pWebContext, Map* pMap);

		bool			Insert(XNode* pxInsert, WebContext* pWebContext, Map* pMap);
		g_int			Delete(XNode* pxDelete, WebContext* pWebContext, Map* pMap);


		g_uint			Insert(XNodeSet* pxNodeSet, WebContext* pWebContext, FeatureWorkspace* pWorkspace);
		g_uint			Update(XNodeSet* pxNodeSet, WebContext* pWebContext, FeatureWorkspace* pWorkspace);
		g_uint			Delete(XNodeSet* pxNodeSet, WebContext* pWebContext, FeatureWorkspace* pWorkspace);
		bool			Insert(XNode* pxInsert, WebContext* pWebContext, FeatureWorkspace* pWorkspace);
		g_int			Update(XNode* pxUpdate, WebContext* pWebContext, FeatureWorkspace* pWorkspace);
		g_int			Delete(XNode* pxDelete, WebContext* pWebContext, FeatureWorkspace* pWorkspace);

		GValue*			CreateValue(XNode* pxNode, GField* pField);

	private:
		WebResponse*	ExecuteByMap(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);
		WebResponse*	ExecuteBySource(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);
	};
}

#endif //__AUGE_WFS_TRANSACTION_HANDLER_H__

