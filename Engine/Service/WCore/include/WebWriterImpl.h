#ifndef __AUGE_WEB_WRITER_H__
#define __AUGE_WEB_WRITER_H__

#include "AugeWebCore.h"

namespace auge
{
	class WebWriterImpl : public WebWriter
	{
	public:
		WebWriterImpl();
		virtual ~WebWriterImpl();
	public:
		virtual RESULTCODE		WriteHead(const char* content_type);
		virtual void			WriteTail();
		virtual RESULTCODE		Write(g_uchar* data, g_uint size);
	};
}

#endif //__AUGE_WEB_WRITER_H__
