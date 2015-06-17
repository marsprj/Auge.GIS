#ifndef __AUGE_CURSOR_CSV_H__
#define __AUGE_CURSOR_CSV_H__

#include "AugeData.h"
#include <fstream>
#include <string>

namespace auge
{
	class DataSetCsv;

	class CursorCsv : public Cursor
	{
	public:
		CursorCsv();
		virtual ~CursorCsv();
	public:
		virtual Row*				NextRow();
		virtual DataSet*			GetDataSet();
		virtual	void				Release();

	public:
		void	Create(DataSetCsv* pDataset);

	private:
		DataSetCsv		*m_pDataset;
		std::ifstream	m_is;
		std::string		m_values;
	};
}

#endif// __AUGE_CURSOR_CSV_H__
