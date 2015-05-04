#include <fcgi_stdio.h>
#include <fcgi_config.h>
#include <rude/cgi.h>

#ifdef WIN32
#	define	AUGE_FCGI_ACCEPT	FCGI_Accept_t(&environ)
#else
#	define	AUGE_FCGI_ACCEPT	FCGI_Accept()
#endif

#ifdef WIN32
#	define	AUGE_RUDE_CGI	rude::CGI cgi(&environ)
#else 
#	define	AUGE_RUDE_CGI	rude::CGI cgi
#endif

void printf_variable(const char* key, const char* value);
void printf_cgi_variable(const char* key, const char* value);

int main(int argc, char* argv[]) 
{
	int count = 0;
	//while(FCGI_Accept() >= 0) 
	//while(FCGI_Accept_t(&environ) >= 0) 
	while(AUGE_FCGI_ACCEPT>=0)
	{ 
		//rude::CGI cgi; 
		//rude::CGI cgi(&environ);
		AUGE_RUDE_CGI;

		printf("ContentType: text/html \r\n" "\r\n");
		//printf("%d \r\n", count++);  

		int count = cgi.numValues();
		printf("[count]:%d<br>", count);
		for(int i=0; i<count; i++) 
		{
			printf_variable(cgi.fieldnameAt(i), cgi[cgi.fieldnameAt(i)]);
			
		}
		printf("=====================================================<br>", count);
		const char* key = "HTTP_COOKIE";
		printf_cgi_variable(key, getenv(key));
		key = "REQUEST_METHOD";
		printf_cgi_variable(key, getenv(key));

		printf("\n\n");
		cgi.finish(); 
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
