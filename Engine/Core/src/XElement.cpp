#include "AugeXML.h"
#include "AugeCore.h"
#include "string.h"

namespace auge
{

	XElement::XElement(xmlNodePtr pxNode):
	XNode(pxNode)
	{

	}

	XElement::~XElement()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	void XElement::SetNamespaceDeclaration(const char* ns_uri, const char* ns_prefix)
	{
		xmlNewNs(m_pxNode, (const xmlChar*)ns_uri,
			(const xmlChar*)ns_prefix);
	}

	const char* XElement::GetNamespaceUriForPrefix(const char* ns_prefix)
	{
		const char* uri = NULL;
		const xmlNs* ns = xmlSearchNs(m_pxNode->doc, const_cast<xmlNode*>(m_pxNode), (xmlChar*)ns_prefix);
		if(ns!=NULL)
		{
			uri = (const char*)ns->prefix;
		}
		return uri;
	}

	XAttribute*	XElement::SetAttribute(const char* name, const char* value, const char* ns_prefix, bool encoding/*=false*/)
	{
		xmlAttr* attr = NULL;

		//char* name_utf8	 = (char*)name;
		//char* value_utf8 = (char*)value;
		//char* ns_prefix_utf8 = (char*)ns_prefix;

		//Ignore the namespace if none was specified: 
		if((ns_prefix==NULL)||strlen(ns_prefix)==0)
		{
			if(encoding)
			{	
				size_t size_utf8 = AUGE_PATH_MAX;
				char name_utf8[AUGE_PATH_MAX];
				char value_utf8[AUGE_PATH_MAX];
				memset(name_utf8, 0, AUGE_PATH_MAX);
				memset(value_utf8, 0, AUGE_PATH_MAX);
				auge_encoding_convert_2("GBK", "UTF-8",name, strlen(name), (char*)name_utf8, &size_utf8);
				auge_encoding_convert_2("GBK", "UTF-8",value, strlen(value), (char*)value_utf8, &size_utf8);
				attr = xmlSetProp(m_pxNode, (const xmlChar*)name_utf8, (const xmlChar*)value_utf8);
			}
			else
			{
				attr = xmlSetProp(m_pxNode, (const xmlChar*)name, (const xmlChar*)value);
			}
		}
		else
		{
			//If the namespace exists, then use it:
			xmlNs* ns = xmlSearchNs(m_pxNode->doc, m_pxNode, (const xmlChar*)ns_prefix);
			if (ns)
			{
				if(encoding)
				{	
					size_t size_utf8 = 0;
					char name_utf8[AUGE_PATH_MAX];
					char value_utf8[AUGE_PATH_MAX];
					memset(name_utf8, 0, AUGE_PATH_MAX);
					memset(value_utf8, 0, AUGE_PATH_MAX);
					auge_encoding_convert_2("GBK", "UTF-8",name, strlen(name), (char*)name_utf8, &size_utf8);
					auge_encoding_convert_2("GBK", "UTF-8",value, strlen(value), (char*)value_utf8, &size_utf8);
					attr = xmlSetProp(m_pxNode, (const xmlChar*)name_utf8, (const xmlChar*)value_utf8);
				}
				else
				{
					attr = xmlSetNsProp(m_pxNode, ns, (const xmlChar*)name, (const xmlChar*)value);
				}
				//attr = xmlSetNsProp(m_pxNode, ns, (const xmlChar*)name_utf8, (const xmlChar*)value_utf8);
			}
			else
			{
			}
		}

		if(attr)
			return reinterpret_cast<XAttribute*>(attr->_private);
		return NULL;
	}

	XAttribute*	XElement::GetAttribute(const char* name, const char* ns_prefix)
	{
		if(name==NULL)
		{
			return NULL;
		}

		const char* name_utf8 = auge_encoding_convert("GBK", "UTF-8", name, strlen(name));
		if(name_utf8==NULL)
		{
			return NULL;
		}

		if(ns_prefix==NULL)
		{
			xmlAttr* attr = xmlHasProp(const_cast<xmlNode*>(m_pxNode), (const xmlChar*)name_utf8);
			if( attr )
			{
				return reinterpret_cast<XAttribute*>(attr->_private);
			}
		}
		else
		{
			const char* ns_uri = GetNamespaceUriForPrefix(ns_prefix);
			xmlAttr* attr = xmlHasNsProp(const_cast<xmlNode*>(m_pxNode), 
				(const xmlChar*)name_utf8,
				(const xmlChar*)ns_uri);
			if( attr )
			{
				return reinterpret_cast<XAttribute*>(attr->_private);
			}
		}

		return NULL;
	}

	void XElement::SetChildText(const char* content, bool encoding/*=false*/)
	{
		XTextNode* node = GetChildText();
		if(node!=NULL)
		{
			if(encoding)
			{	
				size_t content_len = strlen(content);
				size_t size_utf8 = content_len << 1;
				char* content_utf8 = new char[size_utf8];
				memset(content_utf8, 0, size_utf8);
				auge_encoding_convert_2("GBK", "UTF-8",content, content_len, (char*)content_utf8, &size_utf8);
				node->SetContent(content_utf8);
				delete[] content_utf8;
			}
			else
			{
				node->SetContent(content);
			}
		}
		else
		{
			AddChildText(content, encoding);
		}
	}

	XCommentNode* XElement::AddChildComment(const char* content)
	{
		char* content_utf8 = (char*)content;
		xmlNode* node = xmlNewComment((const xmlChar*)content_utf8);

		// Use the result, because node can be freed when merging text nodes:
		node = xmlAddChild(m_pxNode, node);
		return static_cast<XCommentNode*>(node->_private);
	}

	XTextNode* XElement::AddChildText(const char* content, bool encoding/*=false*/)
	{
		if(m_pxNode->type == XML_ELEMENT_NODE)
		{
			xmlNode* node = NULL;
			size_t content_len = strlen(content);
			if(content_len>0)
			{
				if(encoding)
				{	
					size_t size_utf8 = content_len << 1;

					char* content_utf8 = new char[size_utf8];
					memset(content_utf8, 0, size_utf8);
					auge_encoding_convert_2("GBK", "UTF-8",content, content_len, (char*)content_utf8, &size_utf8);
					node = xmlNewText((const xmlChar*)content_utf8);
					delete[] content_utf8;
				}
				else
				{
					node = xmlNewText((const xmlChar*)content);
				}
			}
			else
			{
				node = xmlNewText((const xmlChar*)"");
			}
			
			node = xmlAddChild(m_pxNode, node); 

			return static_cast<XTextNode*>(node->_private);
		}
		return 0;
	}

	XTextNode* XElement::GetChildText()
	{
		// TODO: This only returns the first content node.
		// What should we do instead? Update the documentation if we change this. murrayc.
		for(xmlNode* child = m_pxNode->children; child; child = child->next)
		{
			if(child->type == XML_TEXT_NODE)
			{
				return static_cast<XTextNode*>(child->_private);
			}
		}

		return NULL;
	}

}

