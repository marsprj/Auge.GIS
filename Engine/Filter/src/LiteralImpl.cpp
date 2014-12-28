#include "LiteralImpl.h"

namespace auge
{
	LiteralImpl::LiteralImpl():
	m_pValue(NULL)
	{

	}

	LiteralImpl::~LiteralImpl()
	{
		if(m_pValue!=NULL)
		{
			m_pValue->Release();
			m_pValue = NULL;
		}
	}

	augeExpressionType LiteralImpl::GetType()
	{
		return augeExpLiteral;
	}

	bool LiteralImpl::SetValue(GValue* pValue)
	{
		if(pValue==NULL)
		{
			return false;
		}
		if(m_pValue!=NULL)
		{
			m_pValue->Release();
			m_pValue = NULL;
		}
		m_pValue = pValue;
		if(pValue!=NULL)
		{
			pValue->AddRef();
		}
		return true;
	}


	GValue* LiteralImpl::GetValue()
	{
		return m_pValue;
	}

	void LiteralImpl::Release()
	{
		if(ReleaseRef()<=0)
		{
			delete this;
		}
	}

}
