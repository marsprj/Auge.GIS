#include "AugeXML.h"

namespace auge
{

	XNodeSet::XNodeSet()
	{
		m_iter = m_nodes.begin();
	}

	XNodeSet::~XNodeSet()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	int	XNodeSet::Count()
	{
		return m_nodes.size();
	}

	XNode* XNodeSet::Item(int index)
	{
		if(index<0||index>=m_nodes.size())
			return NULL;
		return m_nodes[index];
	}

	void XNodeSet::Add(XNode* pagNode)
	{
		if(pagNode!=NULL)
			m_nodes.push_back(pagNode);
	}

	void XNodeSet::Release()
	{
		delete this;
	}

	bool XNodeSet::IsEOF()
	{
		return (m_iter==m_nodes.end());
	}

	XNode* XNodeSet::Next()
	{
		if(m_iter==m_nodes.end())
		{
			return NULL;
		}
		return *(m_iter++);
	}

	void XNodeSet::Reset()
	{
		m_iter = m_nodes.begin();
	}

}
