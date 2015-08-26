#ifndef __AUGE_FEATURE_MULTIPOINT_TO_POINTS_PROCESSOR_H__
#define __AUGE_FEATURE_MULTIPOINT_TO_POINTS_PROCESSOR_H__

#include <string>
#include "AugeProcessor.h"

namespace auge
{
	class Feature;
	class FeatureClass;
	class FeatureWorkspace;

	class MultiPointToPointsProcessorImpl : public MultiPointToPointsProcessor
	{
	public:
		MultiPointToPointsProcessorImpl();
		virtual ~MultiPointToPointsProcessorImpl();
	public:

		virtual void		SetUser(g_uint user);

		virtual	void		SetInputDataSource(const char* sourceName);
		virtual	void		SetInputFeatureClass(const char* className);

		virtual	void		SetOutputDataSource(const char* sourceName);
		virtual	void		SetOutputFeatureClass(const char* className);

		virtual RESULTCODE	Execute();
		virtual void		Release();

	private:
		const char*			GetInputSource();
		const char*			GetInputFatureClass();

		const char*			GetOutputSource();
		const char*			GetOutputFatureClass();

		FeatureClass*		CreateOutputFeatureClass(const char* className, FeatureWorkspace* pWorkspace, FeatureClass* pinFeatureClass);
		RESULTCODE			Process(FeatureClass* pinFeatureClass, FeatureClass* poutFeatureClass);
		void				ProcessMultiPoint(Feature* pinFeature, FeatureClass* poutFeatureClass, FeatureInsertCommand* cmd);


	private:
		std::string	m_in_source_name;
		std::string	m_in_class_name;

		std::string m_out_source_name;
		std::string m_out_class_name;

		SrsBase		m_srsbase;

		g_uint		m_user;
	};
}

#endif //__AUGE_FEATURE_MULTIPOINT_TO_POINTS_PROCESSOR_H__
