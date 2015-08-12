#ifndef __AUGE_FEATURE_EXPORT_PROCESSOR_H__
#define __AUGE_FEATURE_EXPORT_PROCESSOR_H__

#include "AugeProcessor.h"
#include <string>

namespace auge
{
	class FeatureExportProcessorImpl : public FeatureExportProcessor
	{
	public:
		FeatureExportProcessorImpl();
		virtual ~FeatureExportProcessorImpl();
	public:

		virtual void		SetUser(g_uint user);

		virtual void		SetShapePath(const char* path);
		virtual void		SetShapeName(const char* className);
		virtual void		SetDataSourceName(const char* sourceName);
		virtual void		SetFeatureClassName(const char* className);

		virtual RESULTCODE	Execute();
		
		const char*			GetShapePath();
		const char*			GetShapeName();
		const char*			GetDataSourceName();
		const char*			GetFeatureClassName();

	private:
		std::string m_shp_path;
		std::string m_shp_name;
		std::string m_db_source_name;
		std::string m_db_feature_class_name;

		g_uint		m_user;
	};
}

#endif //__AUGE_FEATURE_EXPORT_PROCESSOR_H__
