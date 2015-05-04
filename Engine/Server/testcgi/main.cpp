#include <fcgi_stdio.h>
#include <fcgi_config.h>
#include <rude/cgi.h>
#include <stdlib.h>

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

void printf_variable(const char* key, const char* value);
void printf_cgi_variable(const char* key, const char* value);

const char* keys[] = {"AUTH_TYPE",
	"GATEWAY_INTERFACE",
	"PATH_INFO",
	"PATH_TRANSLATED",
	"QUERY_STRING",
	"REMOTE_ADDR",
	"REMOTE_HOST",
	"REMOTE_IDENT",
	"REMOTE_PORT",
	"REMOTE_USER",
	"REQUEST_URI",
	"REQUEST_METHOD",
	"SCRIPT_NAME",
	"SCRIPT_FILENAME",
	"SERVER_ADMIN",
	"SERVER_NAME",
	"SERVER_PORT",
	"SERVER_PROTOCOL",
	"SERVER_SIGNATURE",
	"SERVER_SOFTWARE",
	"HTTP_ACCEPT",
	"HTTP_ACCEPT_ENCODING",
	"HTTP_ACCEPT_LANGUAGE",
	"HTTP_COOKIE",
	"HTTP_FORWARDED",
	"HTTP_HOST",
	"HTTP_PRAGMA",
	"HTTP_REFERER",
	"HTTP_USER_AGENT"};

int main(int argc, char* argv[]) 
{
	int count = 0;
	while(FCGI_Accept() >= 0) 
	//while(FCGI_Accept_t(&environ) >= 0) 
	//while(AUGE_FCGI_ACCEPT>=0)
	{ 
		//rude::CGI cgi; 
		//rude::CGI cgi(&environ);
		//AUGE_RUDE_CGI;

		printf("ContentType: text/html \r\n" "\r\n");
		////printf("%d \r\n", count++);  

		//int count = cgi.numValues();
		//printf("[count]:%d<br>", count);
		//for(int i=0; i<count; i++) 
		//{
		//	printf_variable(cgi.fieldnameAt(i), cgi[cgi.fieldnameAt(i)]);
		//	
		//}
		//printf("=====================================================<br>", count);

		count = sizeof(keys) / sizeof(char*);
		for(int i=0; i<count; i++)
		{
			const char* key = keys[i];
			printf_cgi_variable(key, getenv(key));
		}

		printf("\n\n");
		//cgi.finish(); 
	} 
	return 0;
}

void printf_variable(const char* key, const char* value) 
{
	printf("[%s]:%s<br>", key, value);
}

void printf_cgi_variable(const char* key, const char* value)
{
	if(value==NULL)
	{
		printf("[%s]:<br>", key);
	}
	else
	{
		printf("[%s]:%s<br>", key, value);
	}
}
