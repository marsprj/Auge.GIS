#ifndef __AUGE_FEATURE_PROJECT_PROCESSOR_H__
#define __AUGE_FEATURE_PROJECT_PROCESSOR_H__

#include <string>
#include "AugeProcessor.h"
#include "Kmean.h"

#include "proj_api.h"

namespace auge
{
	class FeatureClass;
	class FeatureWorksapce;

	class FeatureProjectProcessorImpl : public FeatureProjectProcessor
	{
	public:
		FeatureProjectProcessorImpl();
		virtual ~FeatureProjectProcessorImpl();
	public:
		virtual	void		SetInputDataSource(const char* sourceName);
		virtual	void		SetInputFeatureClass(const char* className);

		virtual	void		SetOutputDataSource(const char* sourceName);
		virtual	void		SetOutputFeatureClass(const char* className);
		virtual void		SetOutputSRID(g_uint srid);

		virtual RESULTCODE	Execute();
		virtual void		Release();

	private:
		const char*			GetInputSource();
		const char*			GetInputFatureClass();

		const char*			GetOutputSource();
		const char*			GetOutputFatureClass();

		FeatureClass*		CreateOutputFeatureClass(const char* className, g_uint srid, FeatureWorksapce* pWorkspace, FeatureClass* pinFeatureClass);
		RESULTCODE			Project(FeatureClass* pinFeatureClass, FeatureClass* poutFeatureClass);

		RESULTCODE			Project(WKBPoint* pWKBPoint, projPJ i_pj, projPJ o_pj);

	private:
		std::string	m_in_source_name;
		std::string	m_in_class_name;

		std::string m_out_source_name;
		std::string m_out_class_name;

		g_uint		m_srid;
	};
}

#endif //__AUGE_FEATURE_PROJECT_PROCESSOR_H__
