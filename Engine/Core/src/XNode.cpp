#include "AugeXML.h"
#include "AugeCore.h"
#include "string.h"
#include <libxml/xpath.h>

namespace auge
{

	XNode::XNode(xmlNodePtr pxmlNode):
	m_pxNode(pxmlNode)
	{
		m_pxNode->_private = this;
	}

	XNode::~XNode()
	{

	}

	xmlNodePtr XNode::cobj()
	{
		return m_pxNode;
	}

	const xmlNodePtr XNode::cobj() const
	{
		return m_pxNode;
	}

	//////////////////////////////////////////////////////////////////////////
	void XNode::SetNamespace(const char* ns_prefix)
	{
		xmlNs* ns = xmlSearchNs(m_pxNode->doc, m_pxNode, (xmlChar*)(ns_prefix));
		if(ns!=NULL)
		{
			xmlSetNs(m_pxNode, ns);
		}
		else
		{

		}
	}

	XElement* XNode::AddChild(const char* name, const char* ns_prefix)
	{
		xmlNodePtr pxmlChild = CreateNewChildNode(name, ns_prefix);
		if(pxmlChild==NULL)
		{
			return NULL;
		}
		xmlNodePtr pxmlNode = xmlAddChild(m_pxNode, pxmlChild);
		if(pxmlNode==NULL)
		{
			return NULL;
		}
		return static_cast<XElement*>(pxmlNode->_private);
	}

	void XNode::RemoveChild(XNode* node)
	{	
		xmlUnlinkNode(node->cobj());
		xmlFreeNode(node->cobj()); //The C++ instance will be deleted in a callback.
	}

	xmlNodePtr XNode::CreateNewChildNode(const char* name, const char* ns_prefix)
	{
		xmlNsPtr ns = NULL;
		if(m_pxNode->type != XML_ELEMENT_NODE)
		{
			return NULL;
		}

		if((ns_prefix!=NULL)&&(strlen(ns_prefix)>0))
		{
			ns = xmlSearchNs(m_pxNode->doc, m_pxNode, (const xmlChar*)ns_prefix);
			if(ns==NULL)
			{
				return NULL;
			}
		}

		return xmlNewNode(ns, (const xmlChar*)name);
	}

	XNodeSet* XNode::Find(const char* xpath)
	{
		if(xpath==NULL)
			return NULL;

		xmlXPathContextPtr pxpathCtxt = xmlXPathNewContext(m_pxNode->doc);
		pxpathCtxt->node = m_pxNode;

		xmlXPathObjectPtr pxpathResult = NULL;
		pxpathResult = xmlXPathEval(BAD_CAST xpath, pxpathCtxt);

		if(pxpathResult==NULL)
		{
			xmlXPathFreeContext(pxpathCtxt);
			return NULL;
		}

		if(pxpathResult->type != XPATH_NODESET)
		{
			xmlXPathFreeObject(pxpathResult);
			xmlXPathFreeContext(pxpathCtxt);

			return NULL;
		}

		XNodeSet *pagXmlNodeSet = NULL;
		xmlNodeSetPtr pxmlNodeSet = pxpathResult->nodesetval;

		if(pxmlNodeSet!=NULL)
		{
			pagXmlNodeSet = new XNodeSet();
			for(int i=0; i<pxmlNodeSet->nodeNr; i++)
			{
				pagXmlNodeSet->Add(static_cast<XNode*>(pxmlNodeSet->nodeTab[i]->_private));
			}
		}


		xmlXPathFreeObject(pxpathResult);
		xmlXPathFreeContext(pxpathCtxt);

		return pagXmlNodeSet;
	}

	const char*	XNode::GetName()
	{
		if(m_name.empty())
		{
			const char* name_gbk = NULL;
			name_gbk = auge_encoding_convert("UTF-8", "GBK", (char*)(m_pxNode->name), strlen((char*)(m_pxNode->name)));
			if(name_gbk!=NULL)
			{
				m_name = name_gbk;
			}
		}
		return m_name.c_str();
	}

	bool XNode::SetName(const char* name)
	{
		if(name==NULL)
			return false;

		const char* name_utf8 = auge_encoding_convert("GBK", "UTF-8", name, strlen(name));
		if(name_utf8==NULL)
			return false;

		m_name = name;
		xmlNodeSetName(m_pxNode, BAD_CAST name_utf8);

		return true;
	}

	const char*	XNode::GetNamespacePrefix()
	{
		if(m_pxNode->type != XML_DOCUMENT_NODE)
		{
			return NULL;
		}
		if(m_pxNode && m_pxNode->ns && m_pxNode->ns->prefix)
		{
			return (char*)m_pxNode->ns->prefix;
		}
		return NULL;
	}

	const char* XNode::GetNamespaceUri()
	{
		if(m_pxNode->type != XML_DOCUMENT_NODE)
		{
			return NULL;
		}
		if(m_pxNode && m_pxNode->ns && m_pxNode->ns->href)
		{
			return (char*)m_pxNode->ns->href;
		}
		return NULL;
	}

	XNodeSet* XNode::GetChildren(const char* name)
	{
		xmlNodePtr child = m_pxNode->children;
		if(!child)
		{
			return NULL;
		}

		const char *name_utf8 = NULL;
		if(name!=NULL)
		{
			name_utf8 = auge_encoding_convert("GBK","UTF-8",name,strlen(name));
		}

		XNodeSet* pagNodeSet = new XNodeSet();

		do
		{
			if(child->_private)
			{
				if((name_utf8==NULL)||(xmlStrcasecmp(BAD_CAST name_utf8, child->name)==0))
				{
					pagNodeSet->Add(reinterpret_cast<XNode*>(child->_private));
				}
			}
			else
			{

			}
		}
		while((child=child->next));

		return pagNodeSet;
	}

	XNode* XNode::GetFirstChild(const char* name)
	{
		xmlNodePtr child = m_pxNode->children;
		if(!child)
		{
			return NULL;
		}

		if(name==NULL)
		{
			if(child->_private)
			{
				return reinterpret_cast<XNode*>(child->_private); 
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			const char *name_utf8 = NULL;
			if(name!=NULL)
			{
				name_utf8 = auge_encoding_convert("GBK","UTF-8",name,strlen(name));
			}

			do
			{
				if(child->_private)
				{
					if((name_utf8==NULL)||(xmlStrcasecmp(BAD_CAST name_utf8, child->name)==0))
					{
						return reinterpret_cast<XNode*>(child->_private);
					}
				}
			}
			while((child=child->next));
		}

		return NULL;
	}

	const XElement* XNode::GetParent() const
	{
		return cobj()->parent && cobj()->parent->type == XML_ELEMENT_NODE ? 
			static_cast<const XElement*>(cobj()->parent->_private) : 0;
	}

	XElement* XNode::GetParent()
	{
		return cobj()->parent && cobj()->parent->type == XML_ELEMENT_NODE ? 
			static_cast<XElement*>(cobj()->parent->_private) : 0;
	}

	XNode* XNode::GetSibling()
	{
		return m_pxNode->next ? static_cast<XNode*>(m_pxNode->next->_private) : 0;
	}

	const char*	XNode::GetContent()
	{
		if(m_pxNode->children==NULL)
		{
			return NULL;
		}

		if(!m_content.empty())
		{
			return m_content.c_str();
		}


		const char* content = (const char*)(m_pxNode->children->content);
		if(content==NULL)
		{
			return NULL;
		}

		size_t len_utf8 = strlen(content);
		size_t len_gbk  = len_utf8;
		char* content_gbk = new char[len_utf8+1];
		memset(content_gbk, 0, len_utf8+1);
		auge_encoding_convert_2("UTF-8", "GBK", content, len_utf8, content_gbk, &len_gbk);

		m_content = content_gbk;
		delete[] content_gbk;
		return m_content.c_str();
	}

	const char*	XNode::ToString()
	{
		xmlBufferPtr pxmlBuffer = xmlBufferCreate();
		if(pxmlBuffer==NULL)
		{
			return NULL;
		}

		int len = xmlNodeDump(pxmlBuffer, m_pxNode->doc, m_pxNode, 0,0);
		if(len<=0)
		{
			xmlBufferFree(pxmlBuffer);
			return NULL;
		}

		m_text = (char*)pxmlBuffer->content;
		xmlBufferFree(pxmlBuffer);
		return m_text.c_str();
	}

	XNode* XNode::ImportNode(XNode* node, bool recursive/* = true*/)
	{
		xmlNode* imported_node = xmlDocCopyNode(const_cast<xmlNode*>(node->cobj()), m_pxNode->doc, recursive);
		if (!imported_node)
		{
			return NULL;
		}

		xmlNode* added_node = xmlAddChild(this->cobj(),imported_node);
		if (!added_node)
		{
			XNode::FreeWrappers(imported_node);
			xmlFreeNode(imported_node);

			return NULL;
		}

		XNode::CreateWrapper(imported_node);
		return static_cast<XNode*>(imported_node->_private);
	}

	void XNode::CreateWrapper(_xmlNode* node)
	{
		if(node->_private)
		{
			//Node already wrapped, skip
			return;
		}

		switch (node->type)
		{
		case XML_ELEMENT_NODE:
			{
				node->_private = new XElement(node);
				break;
			}
		case XML_ATTRIBUTE_NODE:
			{
				node->_private = new XAttribute(node);
				break;
			}
		case XML_TEXT_NODE:
			{
				node->_private = new XTextNode(node);
				break;
			}
		case XML_COMMENT_NODE:
			{
				node->_private = new XCommentNode(node);
				break;
			}
		case XML_CDATA_SECTION_NODE:
			{
				node->_private = new XCdataNode(node);
				break;
			}
		case XML_PI_NODE:
			{
				node->_private = new XProcessingInstructionNode(node);
				break;
			}
		case XML_DTD_NODE:
			{
				node->_private = new XDtd(reinterpret_cast<xmlDtd*>(node));
				break;
			}
			//case XML_ENTITY_NODE:
			//{
			//  assert(0 && "Warning: XML_ENTITY_NODE not implemented");
			//  //node->_private = new xmlpp::ProcessingInstructionNode(node);
			//  break;
			//}
		case XML_ENTITY_REF_NODE:
			{
				node->_private = new XEntityReference(node);
				break;
			}
		case XML_DOCUMENT_NODE:
			{
				// do nothing. For Documents it's the wrapper that is the owner.
				break;
			}
		default:
			{
				// good default for release versions
				node->_private = new XNode(node);
				//std::cerr << G_STRFUNC << "Warning: new node of unknown type created: " << node->type << std::endl;
				break;
			}
		}
	}

	void XNode::FreeWrappers(xmlNode* node)
	{
		if(!node)
			return;

		//Walk the children list
		for(xmlNode* child=node->children; child; child=child->next)
			FreeWrappers(child);

		//Delete the local one
		switch(node->type)
		{
			//Node types that have no properties
		case XML_DTD_NODE:
			delete static_cast<XDtd*>(node->_private);
			node->_private = 0;
			return;
		case XML_ATTRIBUTE_NODE:
		case XML_ELEMENT_DECL:
		case XML_ATTRIBUTE_DECL:
		case XML_ENTITY_DECL:
			delete static_cast<XNode*>(node->_private);
			node->_private = 0;
			return;
		case XML_DOCUMENT_NODE:
			//Do not free now. The Document is usually the one who owns the caller.
			return;
		default:
			delete static_cast<XNode*>(node->_private);
			node->_private = 0;
			break;
		}

		//Walk the attributes list.
		//Note that some "derived" struct have a different layout, so 
		//_xmlNode::properties would be a nonsense value, leading to crashes,
		//(and shown as valgrind warnings), so we return above, to avoid 
		//checking it here.
		for(xmlAttr* attr = node->properties; attr; attr = attr->next)
		{
			FreeWrappers(reinterpret_cast<xmlNode*>(attr));
		}
	}

}
