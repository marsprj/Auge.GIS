#ifndef __AUGE_DATA_SET_CSV_H__
#define __AUGE_DATA_SET_CSV_H__

#include "AugeData.h"
#include <string>
#include <fstream>

namespace auge
{
	class WorkspaceCsv;

	class DataSetCsv : public AttributeDataSet
	{
		friend class CursorCsv;

	public:
		DataSetCsv();
		virtual ~DataSetCsv();
	public:
		virtual const char*		GetName();
		virtual augeDataSetType	GetType();

		virtual GFields*		GetFields();
		virtual GField*			GetField(const char* name);

		virtual Cursor*			GetRows();

	public:
		bool		Create(const char* name, const char* path, WorkspaceCsv* pWorkspace);
		GFields*	CreateFields();

	private:
		WorkspaceCsv*	m_pWorkspace;

		std::string		m_name;
		std::string		m_path;

		GFields*		m_pFields;
	};
}

#endif //__AUGE_DATA_SET_CSV_H__
