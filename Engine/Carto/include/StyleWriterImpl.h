#ifndef __AUGE_STYLE_WRITER_H__
#define __AUGE_STYLE_WRITER_H__

#include "AugeStyle.h"
#include "AugeXML.h"

namespace auge
{
	class StyleWriterImpl : public StyleWriter
	{
	public:
		StyleWriterImpl();
		virtual ~StyleWriterImpl();
	public:
		virtual XDocument*		Write(Style* pStyle, const char* version="1.0.0");
	};

}

#endif //__AUGE_STYLE_WRITER_H__

