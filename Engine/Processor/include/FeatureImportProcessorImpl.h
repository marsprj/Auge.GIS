#ifndef __AUGE_FEATURE_IMPORT_PROCESSOR_H__
#define __AUGE_FEATURE_IMPORT_PROCESSOR_H__

#include "AugeProcessor.h"
#include <string>

namespace auge
{
	class FeatureImportProcessorImpl : public FeatureImportProcessor
	{
	public:
		FeatureImportProcessorImpl();
		virtual ~FeatureImportProcessorImpl();
	public:

		virtual void		SetUser(User* pUser);

		virtual void		SetShapePath(const char* path);
		virtual void		SetShapeName(const char* className);
		virtual void		SetDataSourceName(const char* sourceName);
		virtual void		SetTypeName(const char* typeName);

		virtual RESULTCODE	Execute();
		
		const char*			GetShapePath();
		const char*			GetShapeName();
		const char*			GetDataSourceName();
		const char*			GetTypeName();

	private:
		std::string m_shp_path;
		std::string m_shp_name;
		std::string m_db_source_name;
		std::string m_db_type_name;

		User*		m_pUser;
	};
}

#endif //__AUGE_FEATURE_IMPORT_PROCESSOR_H__
