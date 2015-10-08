#ifndef __AUGE_CSV_IMPORT_HANDLER_H__
#define __AUGE_CSV_IMPORT_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "GProcessHandler.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class WebContext;
	class CsvImportRequest;
	class FeatureClass;

	class CsvImportHandler : public GProcessHandler
	{
	public:
		CsvImportHandler();
		virtual ~CsvImportHandler();
	public:
		virtual const char*		GetName();
		virtual const char*		GetDescription();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);

	protected:
		WebResponse*	ImportCsvFile(const char* csv_path, const char* csv_name, const char* source_name, const char* type_name, g_int user_id);
	};
}

#endif //__AUGE_CSV_IMPORT_HANDLER_H__
