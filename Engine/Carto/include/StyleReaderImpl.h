#ifndef __AUGE_STYLE_READER_H__
#define __AUGE_STYLE_READER_H__

#include "AugeStyle.h"
#include "AugeXML.h"

namespace auge
{
	class FeatureClass;

	class StyleReaderImpl : public StyleReader
	{
	public:
		StyleReaderImpl();
		virtual ~StyleReaderImpl();
	public:
		virtual	Style*	Read(const char* path, FeatureClass* pFeatureClass);
		virtual	Style*	Read(const char* buffer, int length, FeatureClass* pFeatureClass);
		virtual void	Release();

	private:
		Style* Read_1_0_0(XElement* pxRoot, FeatureClass* pFeatureClass);
		Style* Read_1_1_0(XElement* pxRoot, FeatureClass* pFeatureClass);

	private:
		XDocument	*m_pxDoc;
	};

}

#endif //__AUGE_STYLE_READER_H__

