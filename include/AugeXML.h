#ifndef __AUGE_XML_H__
#define __AUGE_XML_H__

#ifdef WIN32
#	ifdef AUGE_EXPORTS
#		define AUGE_XML_API __declspec(dllexport)
#	else
#		define AUGE_XML_API __declspec(dllimport)
#	endif
#else
#	define AUGE_XML_API
#endif

#include <AugeType.h>
#include <AugeCore.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string>
#include <vector>

namespace auge
{
	class XNonCopyable;
	class XDocument;
	class XNode;
	class XContentNode;
	class XTextNode;
	class XCdataNode;
	class XCommentNode;
	class XElement;
	class XDtd;
	class XNodeSet;
	class XParser;

	class AUGE_XML_API XNonCopyable : public GObject
	{
	protected:
		XNonCopyable(){}
		virtual ~XNonCopyable(){}

	//private:
	//	XNonCopyable(const XNonCopyable&);
	//	XNonCopyable& operator=(const XNonCopyable&);
	};

	class AUGE_XML_API XDocument : public XNonCopyable
	{
		class Init
		{
		public:
			Init();
			virtual ~Init();
		};

	public:
		explicit XDocument(const char* version="1.0");
		virtual ~XDocument();

	public:
		long			Parse(const char* szPath);
		long			ParseMemory(const char* content);
		long			Save(const char* szPath, const char* szEncoding=NULL, int format=0);
		long			WriteToString(unsigned char** ppBuffer, int& length , const char* szEncoding=NULL, int format=0);
		long			Close();

		void			SetKeepBlanks(bool flag);

		XElement*		CreateRootNode(const char* szName, const char* ns_uri, const char* ns_prefix);
		XElement*		GetRootNode();

		void			SetInternalSubset(const char* name,	const char* external_id, const char* system_id);

		void			Release();

		XDocument(xmlDocPtr pxmlDoc);
	private:
		xmlDocPtr	m_pxDoc;
		std::string	m_strPath;
		xmlChar*	m_pBuffer;

		static Init init_;
	};

	//------------------------------------------------------------------------
	class AUGE_XML_API XNode : public XNonCopyable
	{
	public:
		explicit XNode(xmlNodePtr node);
		virtual ~XNode();
	public:
		const char*		GetName();
		bool			SetName(const char* name);

		const char*		GetNamespacePrefix();
		const char*		GetNamespaceUri();
		void			SetNamespace(const char* ns_prefix);

		XNodeSet*		Find(const char* xpath);
		XNodeSet*		GetChildren(const char* name = NULL);
		XNode*			GetFirstChild(const char* name = NULL);

		XElement*		GetParent();
		const XElement* GetParent() const;

		XNode*			GetSibling();

		XElement*		AddChild(const char* name, const char* ns_prefix=NULL);
		void			RemoveChild(XNode* node);
		XNode*			ImportNode(XNode* node, bool recursive = true);

		const char*		GetContent();

		const char*		ToString();

		xmlNodePtr		cobj();
		const xmlNodePtr cobj() const;

	public:
		static void		CreateWrapper(xmlNodePtr node);
		static void		FreeWrappers(xmlNodePtr node);

	protected:
		xmlNodePtr		CreateNewChildNode(const char* name, const char* ns_prefix);
	protected:
		xmlNodePtr		m_pxNode;

		std::string		m_name;
		std::string		m_content;
		std::string		m_text;
	};
	//------------------------------------------------------------------------
	class AUGE_XML_API XContentNode : public XNode
	{
	public:
		explicit XContentNode(xmlNodePtr node);
		virtual ~XContentNode();
	public:
		void			SetContent(const char* content);
		const char*		GetContent();
	private:
		std::string m_content;
	};

	class AUGE_XML_API XAttribute : public XNode
	{
	public:
		explicit XAttribute(xmlNodePtr node);
		virtual ~XAttribute();
	public:
		const char*		GetName();	
		const char*		GetValue();
		void			SetValue(const char* value);
	private:
		std::string m_value;
		std::string m_name;
	};

	//------------------------------------------------------------------------
	class AUGE_XML_API XTextNode : public XContentNode
	{
	public:
		explicit XTextNode(xmlNodePtr node);
		virtual ~XTextNode();
	};

	//------------------------------------------------------------------------
	class AUGE_XML_API XCdataNode : public XContentNode
	{
	public:
		explicit XCdataNode(xmlNodePtr node);
		virtual ~XCdataNode();
	};

	//------------------------------------------------------------------------
	class AUGE_XML_API XCommentNode : public XContentNode
	{
	public:
		explicit XCommentNode(xmlNodePtr node);
		virtual ~XCommentNode();
	};

	//------------------------------------------------------------------------
	class AUGE_XML_API XEntityReference : public XContentNode
	{
	public:
		XEntityReference(xmlNodePtr node);
		virtual ~XEntityReference();
	};

	//------------------------------------------------------------------------
	class AUGE_XML_API XProcessingInstructionNode : public XContentNode
	{
	public:
		explicit XProcessingInstructionNode(xmlNodePtr node);
		virtual ~XProcessingInstructionNode();
	};

	//------------------------------------------------------------------------
	class AUGE_XML_API XElement : public XNode
	{
	public:
		explicit XElement(xmlNodePtr node);
		virtual ~XElement();
	public:
		void				SetNamespaceDeclaration(const char* ns_uri, const char* ns_prefix);
		//void				SetNamespace(const char* ns_prefix);
		const char*			GetNamespaceUriForPrefix(const char* ns_prefix);

		XAttribute*			SetAttribute(const char* name, const char* value, const char* ns_prefix);
		XAttribute*			GetAttribute(const char* name, const char* ns_prefix=NULL);

		void				SetChildText(const char* content);
		XTextNode*			GetChildText();
		XTextNode*			AddChildText(const char* content, bool encoding=false);

		XCommentNode*		AddChildComment(const char* content);
	};

	//------------------------------------------------------------------------
	class AUGE_XML_API XNodeSet
	{
	public:
		XNodeSet();
		virtual ~XNodeSet();
	public:
		int					Count();
		XNode*				Item(int index);
		void				Add(XNode* pagNode);

		bool				IsEOF();
		XNode*				Next();
		void				Reset();

		void				Release();
	private:
		std::vector<XNode*>	m_nodes;
		std::vector<XNode*>::iterator	m_iter;
	};
	//------------------------------------------------------------------------
	class AUGE_XML_API XParser
	{
	public:
		XParser();
		virtual ~XParser();
	public:
		XDocument*			Parse(const char* szPath, bool keep_blank=false);
		XDocument*			ParseMemory(const char* content, bool keep_blank=false);
	private:
		bool				ParseContext(xmlParserCtxtPtr pxmlParseCtxt);
	};

	//------------------------------------------------------------------------

	class AUGE_XML_API XDtd
	{
	public:
		XDtd(xmlDtdPtr pxmldtd);
		virtual ~XDtd();
	public:
		const char*			GetName() const;
		const char*			GetExternalId() const;
		const char*			GetSystemId() const;
	private:
		xmlDtdPtr	m_pxDtd;
	};
}


#endif //__AUGE_XML_H__
