#include "StyleWriterImpl.h"
#include "StyleWriter_1_0_0.h"

namespace auge
{
	StyleWriterImpl::StyleWriterImpl()
	{

	}

	StyleWriterImpl::~StyleWriterImpl()
	{

	}

	XDocument* StyleWriterImpl::Write(Style* pStyle, const char* version/* ="1.0.0" */)
	{
		if(pStyle==NULL||version==NULL)
		{
			return NULL;
		}

		XDocument* pxDoc = NULL;
		if(strcmp(version, "1.0.0")==0)
		{
			StyleWriter_1_0_0 writer;
			pxDoc = writer.Write(pStyle);
		}
		else if(strcmp(version, "1.1.0")==0)
		{

		}
		return pxDoc;
	}
}