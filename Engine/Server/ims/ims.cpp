//#include <fcgi_stdio.h>
//#include <fcgi_config.h>
//#include <rude/cgi.h>

#include "GServer.h"

int main()
{
	auge::GServer server;

	if(!server.Startup())
	{
		server.Shutdown();
	}

	server.Run();

	server.Shutdown();
}