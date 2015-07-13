#include "AugeStyle.h"
#include "MarkImpl.h"

namespace auge
{ 

	MarkImpl::MarkImpl():
m_type(augeMarkerSquare),
	m_pFill(NULL),
	m_pStroke(NULL)
{

}

MarkImpl::~MarkImpl()
{
	if(m_pFill!=NULL)
	{
		m_pFill->Release();
		m_pFill = NULL;
	}
	if(m_pStroke!=NULL)
	{
		m_pStroke->Release();
		m_pStroke = NULL;
	}
}

augeMarkerType MarkImpl::GetType()
{
	return m_type;
}

void MarkImpl::SetType(augeMarkerType type)
{
	m_type = type;
}

Fill* MarkImpl::GetFill()
{
	return m_pFill;
}

bool MarkImpl::SetFill(Fill *pFill)
{
	if(m_pFill!=NULL)
	{
		m_pFill->Release();
		m_pFill = NULL;
	}
	m_pFill = pFill;
	return true;
}

Stroke* MarkImpl::GetStoke()
{
	return m_pStroke;
}

bool MarkImpl::SetStroke(Stroke *pStroke)
{
	if(pStroke!=NULL)
	{
		m_pStroke->Release();
		m_pStroke = NULL;
	}
	m_pStroke = pStroke;
	return true;
}

void MarkImpl::Release()
{
	delete this;
}

}
