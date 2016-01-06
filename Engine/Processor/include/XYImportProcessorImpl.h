#ifndef __AUGE_XY_IMPORT_PROCESSOR_H__
#define __AUGE_XY_IMPORT_PROCESSOR_H__

#include <string>
#include "AugeProcessor.h"

namespace auge
{
	class GFields; 
	class FeatureClass;
	class FeatureInsertCommand;
	class Row;

	class XYImportProcessorImpl : public XYImportProcessor
	{
	public:
		XYImportProcessorImpl();
		virtual ~XYImportProcessorImpl();
	public:
		virtual void		SetUser(g_uint user);

		virtual	void		SetXYPath(const char* path);
		virtual void		SetDataSource(const char* sourceName);
		virtual void		SetDatasetName(const char* datasetName);
		virtual void		SetX(const char* fname);
		virtual void		SetY(const char* fname);
		virtual RESULTCODE	Execute();
		virtual void		Release();

	private:
		bool				IsMatch(GFields* pcsvFields, GFields* pobjFields);
		bool				AddFeature(Row* pRow, FeatureClass* pFeatureClass, FeatureInsertCommand* cmd);

	private:
		std::string	m_xy_path;
		std::string m_source_name;
		std::string m_dataset_name;
		std::string m_x_field;
		std::string m_y_field;

		g_uint			m_user;
	};
}

#endif //__AUGE_XY_IMPORT_PROCESSOR_H__
