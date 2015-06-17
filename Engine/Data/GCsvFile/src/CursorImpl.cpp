#include "RowImpl.h"
#include "CursorImpl.h"
#include "DataSetCvs.h"

namespace auge
{
	CursorCsv::CursorCsv()
	{

	}

	CursorCsv::~CursorCsv()
	{
		if(m_is.is_open())
		{
			m_is.close();
		}
	}

	Row* CursorCsv::NextRow()
	{
		if(!getline(m_is, m_values))
		{
			return NULL;
		}

		RowCsv* pRow = new RowCsv();
		pRow->Create(m_values.c_str(), m_pDataset);
		return pRow;
	}

	DataSet* CursorCsv::GetDataSet()
	{
		return m_pDataset;
	}

	void CursorCsv::Create(DataSetCsv* pDataset)
	{
		m_pDataset = pDataset;

		m_is.open(m_pDataset->m_path.c_str());
		getline(m_is, m_values);
	}

	void CursorCsv::Release()
	{
		if(ReleaseRef()<=0)
		{
			delete this;
		}
	}
}