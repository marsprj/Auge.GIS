#include "EnumDataSetImpl.h"

namespace auge
{
	EnumDataSetImpl::EnumDataSetImpl()
	{
		m_iter = m_datasets.begin();
	}

	EnumDataSetImpl::~EnumDataSetImpl()
	{
		DataSet* pDataSet = NULL;
		std::vector<DataSet*>::iterator iter;
		for(iter=m_datasets.begin(); iter!=m_datasets.end(); iter++)
		{
			pDataSet = *iter;
			pDataSet->Release();
		}
		m_datasets.clear();
	}

	void EnumDataSetImpl::Reset()
	{
		m_iter = m_datasets.begin();
	}

	DataSet* EnumDataSetImpl::Next()
	{
		if(m_iter==m_datasets.end())
		{
			return NULL;
		}
		return *(m_iter++);
	}

	bool EnumDataSetImpl::Add(DataSet* pDataSet)
	{
		if(pDataSet)
		{
			m_datasets.push_back(pDataSet);
		}
		return true;
	}

	void EnumDataSetImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}
}