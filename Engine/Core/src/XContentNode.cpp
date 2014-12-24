#include "AugeXML.h"
#include "AugeCore.h"
#include "string.h"

namespace auge
{

	XContentNode::XContentNode(xmlNodePtr pxmlNode):
	XNode(pxmlNode)
	{

	}

	XContentNode::~XContentNode()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	void XContentNode::SetContent(const char* content)
	{
		if(content!=NULL)
			return;
		if(m_pxNode->type == XML_ELEMENT_NODE)
		{
			return;
		}
		/***********************************/
		char* content_utf8 = (char*)content;
		xmlNodeSetContent(m_pxNode, BAD_CAST content_utf8);
	}

	const char*	XContentNode::GetContent()
	{
		if(m_pxNode->type == XML_ELEMENT_NODE)
		{
			return NULL;
		}

		if(!m_content.empty())
		{
			return NULL;
		}

		const char* content = (const char*)(m_pxNode->content);
		if(content==NULL)
		{
			return NULL;
		}

		size_t len_utf8 = strlen(content);
		size_t len_gbk  = 0;
		char* content_gbk = new char[len_utf8+1];
		memset(content_gbk, 0, len_utf8+1);
		auge_encoding_convert_2("UTF-8", "GBK", content, len_utf8, content_gbk, &len_gbk);

		m_content = content_gbk;
		delete[] content_gbk;
		return m_content.c_str();
	}

}
