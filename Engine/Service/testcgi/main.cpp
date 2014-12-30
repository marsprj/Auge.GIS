#include <fcgi_stdio.h>
#include <fcgi_config.h>
#include <rude/cgi.h>

int main()
{
	int count = 0;
	while(FCGI_Accept() >= 0) 
	{
		rude::CGI cgi; 
	
		printf("ContentType: text/plain \r\n" "\r\n");
		printf("\r\n");
		printf("%d \r\n", count++);

		printf("\r\n\r\n");
		
		cgi.finish(); 
	}
	return 0;
}
