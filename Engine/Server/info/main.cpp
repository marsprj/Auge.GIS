#include <fcgi_stdio.h>
#include <fcgi_config.h>
#include <stdlib.h>
//#include <rude/cgi.h>
//
//#ifdef WIN32
//#	define	AUGE_FCGI_ACCEPT	FCGI_Accept_t(&environ)
//#else
//#	define	AUGE_FCGI_ACCEPT	FCGI_Accept()
//#endif
//
//#ifdef WIN32
//#	define	AUGE_RUDE_CGI	rude::CGI cgi(&environ)
//#else 
//#	define	AUGE_RUDE_CGI	rude::CGI cgi
//#endif

void print_cgi_variable(const char* key);

int main()
{
	int counter = 0;
	while(FCGI_Accept()>=0)
	//while(AUGE_FCGI_ACCEPT)
	{
		//AUGE_RUDE_CGI;

		printf("Content-type: text/html\r\n" "\r\n" "%d",counter++);

		print_cgi_variable("SERVER_NAME");
		print_cgi_variable("SERVER_SOFTWARE");
		print_cgi_variable("GATEWAY_INTERFACE");
		print_cgi_variable("SERVER_PROTOCOL");
		print_cgi_variable("SERVER_PORT ");
		print_cgi_variable("REQUEST_METHOD");
		print_cgi_variable("HTTP_ACCEPT");
		print_cgi_variable("HTTP_USER_AGENT");
		print_cgi_variable("HTTP_REFERER");
		print_cgi_variable("PATH_INFO");
		print_cgi_variable("PATH_TRANSLATED");
		print_cgi_variable("SCRIPT_NAME");
		print_cgi_variable("QUERY_STRING");
		print_cgi_variable("REMOTE_HOST");
		print_cgi_variable("REMOTE_ADDR");
		print_cgi_variable("REMOTE_USER");
		print_cgi_variable("REMOTE_IDENT");
		print_cgi_variable("CONTENT_TYPE");
		print_cgi_variable("CONTENT_LENGTH");

		printf("\r\n\r\n");

		//cgi.finish();
	}
	return 0;
}

void print_cgi_variable(const char* key)
{
	const char* value = getenv(key);
	printf("[%s]:%s<br>\n",key, value==NULL ? "" : value);
}