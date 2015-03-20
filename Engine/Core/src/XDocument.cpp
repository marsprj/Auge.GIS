#include "AugeXML.h"
#include "AugeCore.h"
#include "string.h"
#include "assert.h"

namespace auge
{
	void on_libxml_construct(xmlNode* node);
	void on_libxml_destruct(xmlNode* node);

	XDocument::Init::Init()
	{
		xmlInitParser(); //Not always necessary, but necessary for thread safety.
		xmlRegisterNodeDefault(on_libxml_construct);
		xmlDeregisterNodeDefault(on_libxml_destruct);
		xmlThrDefRegisterNodeDefault(on_libxml_construct);
		xmlThrDefDeregisterNodeDefault(on_libxml_destruct);
	}

	XDocument::Init::~Init()
	{
		xmlCleanupParser(); //As per xmlInitParser(), or memory leak will happen.
	}

	XDocument::Init XDocument::init_;

	XDocument::XDocument(const char* version/*="1.0"*/):
	m_pxDoc(xmlNewDoc((const xmlChar*)version))
	{
		m_pxDoc->_private = this;
		m_pBuffer = NULL;
	}

	XDocument::~XDocument() 
	{
		Close();
		if(m_pxDoc!=NULL)
		{
			xmlFreeDoc(m_pxDoc);
			m_pxDoc = NULL;
		}
	}

	XDocument::XDocument(xmlDocPtr pxmlDoc):
	m_pxDoc(pxmlDoc),
		m_pBuffer(NULL)
	{
		m_pxDoc->_private = this;
	}

	//////////////////////////////////////////////////////////////////////////
	long XDocument::Parse(const char* path)
	{
		return AG_SUCCESS;
	}

	long XDocument::ParseMemory(const char* content)
	{
		return AG_SUCCESS;
	}

	long XDocument::Save(const char* szPath, const char* szEncoding/*=NULL*/, int format/*=0*/)
	{
		char* pmem = NULL;
		int size = 0;
		xmlDocDumpMemory(m_pxDoc, (xmlChar**)&pmem, &size);
		printf("%s\n", pmem);
		int ret = xmlSaveFormatFileEnc(szPath, m_pxDoc, szEncoding, format);
		//int ret = xmlSaveFileEnc(szPath, m_pxDoc, szEncoding);
		return AG_SUCCESS;
	}

	long XDocument::WriteToString(unsigned char** ppBuffer, int& length , const char* szEncoding/*=NULL*/, int format/*=0*/)
	{
		//KeepBlanks k(KeepBlanks::Default);
		xmlIndentTreeOutput = format?1:0;
		*ppBuffer = NULL;

		if(m_pBuffer!=NULL)
		{
			xmlFree(m_pBuffer);
			m_pBuffer = NULL;
		}

		const char* encoding = (szEncoding==NULL) ? "UTF-8" : szEncoding;
		xmlDocDumpFormatMemoryEnc(m_pxDoc, &m_pBuffer, &length, encoding, format?1:0);

		if(m_pBuffer==NULL)
		{
			return AG_FAILURE;
		}

		*ppBuffer = m_pBuffer;

		// Deletes the original buffer
		//xmlFree(buffer);

		return AG_SUCCESS;
	}

	long XDocument::Close()
	{
		if(m_pBuffer != NULL)
		{
			xmlFree(m_pBuffer);
			m_pBuffer = NULL;
		}
		return AG_SUCCESS;
	}

	void XDocument::SetKeepBlanks(bool flag)
	{
		xmlKeepBlanksDefault(flag?1:0);
	}

	XElement* XDocument::CreateRootNode(const char* szName, const char* ns_uri, const char* ns_prefix)
	{
		if(szName==NULL)
		{
			return NULL;
		}

		const char* szNameUtf8 = auge_encoding_convert("GBK", "UTF-8", szName, strlen(szName));
		xmlNodePtr pxmlRootNode = xmlNewDocNode(m_pxDoc, 0, (const xmlChar*)szNameUtf8, 0);
		if(pxmlRootNode==NULL)
		{
			return NULL;
		}
		xmlDocSetRootElement(m_pxDoc, pxmlRootNode);

		XElement* pRootElement = GetRootNode();

		if(ns_uri!=NULL&&pRootElement!=NULL)
		{
			pRootElement->SetNamespaceDeclaration(ns_uri, ns_prefix);
			pRootElement->SetNamespace(ns_prefix);
		}
		return pRootElement;
	}

	XElement* XDocument::GetRootNode()
	{
		xmlNode* root = xmlDocGetRootElement(m_pxDoc);
		if(root == NULL)
			return NULL;

		return reinterpret_cast<XElement*>(root->_private);
	}

	void XDocument::SetInternalSubset(const char* name,	const char* external_id, const char* system_id)
	{
		xmlDtd* dtd = xmlCreateIntSubset(m_pxDoc,
			(const xmlChar*)name,
			(const xmlChar*)external_id,
			(const xmlChar*)system_id);
		if (dtd && !dtd->_private)
		{
			dtd->_private = new XDtd(dtd);
		}
	}

	void XDocument::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void on_libxml_construct(xmlNode* node)
	{
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
				// do nothing ! in case of documents it's the wrapper that is the owner
				break;
			}
		default:
			{
				// good default for release versions
				node->_private = new XNode(node);
				assert(0 && "Warning: new node of unknown type created");
				break;
			}
		}
	}

	//Called by libxml whenever it destroys something
	//such as a node or attribute.
	//This allows us to delete the C++ instance for the C instance, if any.
	void on_libxml_destruct(xmlNode* node)
	{
		bool bPrivateDeleted = false;
		if (node->type == XML_DTD_NODE)
		{
			XDtd* cppDtd = static_cast<XDtd*>(node->_private);
			if(cppDtd)
			{
				delete cppDtd;     
				bPrivateDeleted = true;
			}
		}
		else if (node->type == XML_DOCUMENT_NODE)
			// do nothing. See on_libxml_construct for an explanation
			;
		else
		{
			XNode* cppNode =  static_cast<XNode*>(node->_private);
			if(cppNode)
			{
				delete cppNode;
				bPrivateDeleted = true;
			}
		}

		//This probably isn't necessary:
		if(bPrivateDeleted)
			node->_private = 0;
	}

}
