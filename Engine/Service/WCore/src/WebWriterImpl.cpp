//#ifdef _DEBUG
//#	include "stdio.h"
//#else
#	include <fcgi_stdio.h>
#	include <fcgi_config.h>
//#endif
#include "WebWriterImpl.h"

namespace auge
{
	WebWriter* augeCreateWebWriter()
	{
		return (new WebWriterImpl());
	}

	WebWriterImpl::WebWriterImpl()
	{

	}

	WebWriterImpl::~WebWriterImpl()
	{

	}
	
	RESULTCODE WebWriterImpl::Write(g_uchar* data, g_uint size)
	{
		g_uchar* ptr = data;
		int nbytes = 0;
		int buffer_size = size<4096?size:4096;
		size_t rbytes = size;

		while(rbytes>0)
		{
			nbytes = fwrite(ptr, sizeof(g_uchar), buffer_size, stdout);
			ptr += nbytes;
			rbytes -= nbytes;
			if(rbytes<buffer_size)
			{
				buffer_size = rbytes;
			}
		}
		
		return AG_SUCCESS;
	}

	RESULTCODE WebWriterImpl::WriteHead(const char* content_type)
	{
		if(content_type==NULL)
		{
			return AG_FAILURE;
		}

		printf("Content-Type: %s \r\n" "\r\n", content_type);

		return AG_SUCCESS;
	}

	void WebWriterImpl::WriteTail()
	{
		printf("\r\n \r\n");
	}
}
