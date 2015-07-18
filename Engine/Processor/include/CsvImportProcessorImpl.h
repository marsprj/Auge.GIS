#ifndef __AUGE_CSV_IMPORT_PROCESSOR_H__
#define __AUGE_CSV_IMPORT_PROCESSOR_H__

#include <string>
#include "AugeProcessor.h"

namespace auge
{
	class GFields; 
	class FeatureClass;
	class FeatureInsertCommand;
	class Row;

	class CsvImportProcessorImpl : public CsvImportProcessor
	{
	public:
		CsvImportProcessorImpl();
		virtual ~CsvImportProcessorImpl();
	public:
		virtual void		SetUser(g_uint user);

		virtual	void		SetCvsPath(const char* path);
		virtual void		SetDataSource(const char* sourceName);
		virtual void		SetDatasetName(const char* datasetName);
		virtual RESULTCODE	Execute();
		virtual void		Release();

	private:
		bool				IsMatch(GFields* pcsvFields, GFields* pobjFields);
		bool				AddFeature(Row* pRow, FeatureClass* pFeatureClass, FeatureInsertCommand* cmd);

	private:
		std::string	m_csv_path;
		std::string m_source_name;
		std::string m_dataset_name;

		g_uint			m_user;
	};
}

#endif //__AUGE_CSV_IMPORT_PROCESSOR_H__
