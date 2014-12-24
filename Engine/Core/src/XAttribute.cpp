#include "AugeXML.h"
#include "AugeCore.h"
#include "string.h"

namespace auge
{

	XAttribute::XAttribute(xmlNodePtr pxNode):
	XNode(pxNode)
	{
		m_value = "";
	}

	XAttribute::~XAttribute()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	const char* XAttribute::GetName()
	{
		if(m_name.empty())
		{
			const char* name = (const char*)(m_pxNode->name);
			const char* name_gbk = auge_encoding_convert("UTF-8", "GBK", name, strlen(name));
			m_name = name_gbk;
		}

		return m_name.c_str();
	}	

	const char* XAttribute::GetValue()
	{
		if(!m_value.empty())
		{
			return m_value.c_str();
		}

		const xmlChar* ns_uri = NULL;
		if(m_pxNode->ns!=NULL)
		{
			ns_uri = m_pxNode->ns->href;
		}

		xmlChar *value = xmlGetNsProp(m_pxNode->parent, m_pxNode->name, ns_uri);
		if(value==NULL)
		{
			return NULL;
		}
		const char* value_gbk = auge_encoding_convert("UTF-8", "GBK", (char*)value, strlen((char*)value));
		xmlFree(value);
		m_value = value_gbk;
		return m_value.c_str();
	}

	void XAttribute::SetValue(const char* value)
	{
		if(value!=NULL)
		{
			const char* value_utf8 = auge_encoding_convert("GBK", "UTF-8", value, strlen(value));
			xmlSetProp(m_pxNode->parent, m_pxNode->name, (xmlChar*)value_utf8);
		}
	}

}

