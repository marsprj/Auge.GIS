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
		virtual void		SetShapePath(const char* path);
		virtual void		SetShapeName(const char* className);
		virtual void		SetDataSourceName(const char* sourceName);
		virtual RESULTCODE	Execute();
		
		const char*			GetShapePath();
		const char*			GetShapeName();
		const char*			GetDataSourceName();

	private:
		std::string m_shp_path;
		std::string m_shp_name;
		std::string m_db_source_name;
	};
}

#endif //__AUGE_FEATURE_IMPORT_PROCESSOR_H__
