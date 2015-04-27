#include "RegisterHandler.h"

namespace auge
{
	RegisterHandler::RegisterHandler()
	{

	}

	RegisterHandler::~RegisterHandler()
	{

	}

	const char*	RegisterHandler::GetName()
	{
		return "RegisterHandler";
	}

	WebRequest*	RegisterHandler::ParseRequest(rude::CGI& cgi)
	{
		return NULL;
	}

	WebRequest*	RegisterHandler::ParseRequest(XDocument* pxDoc)
	{
		return NULL;
	}

	WebResponse* RegisterHandler::Execute(WebRequest* pWebRequest)
	{
		return NULL;
	}
}