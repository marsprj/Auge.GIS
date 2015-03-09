
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