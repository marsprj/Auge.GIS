#include "EnumDataSetImpl.h"

namespace auge
{
	EnumDataSetImpl::EnumDataSetImpl()
	{
		m_iter = m_datasets.begin();
	}

	EnumDataSetImpl::~EnumDataSetImpl()
	{
		Cleanup();
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

	bool EnumDataSetImpl::Add(DataSet* pDataset)
	{
		if(pDataset==NULL)
		{
			return false;
		}
		m_datasets.push_back(pDataset);		
		return true;
	}

	void EnumDataSetImpl::Release()
	{
		delete this;
	}

	void EnumDataSetImpl::Cleanup()
	{
		DataSet* pDataset = NULL;
		std::vector<DataSet*>::iterator iter;
		for(iter=m_datasets.begin(); iter!=m_datasets.end(); iter++)
		{
			pDataset = *iter;
			pDataset->Release();
		}
		m_datasets.clear();
	}
}