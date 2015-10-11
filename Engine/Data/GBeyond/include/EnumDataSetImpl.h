#ifndef __AUGE_ENUM_DATASET_IMPL_H__
#define __AUGE_ENUM_DATASET_IMPL_H__

#include "AugeData.h"
#include <vector>

namespace auge
{
	class EnumDataSetImpl : public EnumDataSet
	{
	public:
		EnumDataSetImpl();
		virtual ~EnumDataSetImpl();
	public:
		virtual void			Reset();
		virtual DataSet*		Next();
		virtual bool			Add(DataSet* pDataSet);
		virtual void			Release();

	private:
		std::vector<DataSet*>	m_datasets;
		std::vector<DataSet*>::iterator m_iter;
	};
}

#endif //__AUGE_ENUM_DATASET_IMPL_H__
