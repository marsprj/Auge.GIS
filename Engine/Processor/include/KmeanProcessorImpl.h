#ifndef __AUGE_KMEAN_PROCESSOR_H__
#define __AUGE_KMEAN_PROCESSOR_H__

#include <string>
#include "AugeProcessor.h"
#include "Kmean.h"

namespace auge
{
	class FeatureWorksapce;

	class KMeanProcessorImpl : public KMeanProcessor
	{
	public:
		KMeanProcessorImpl();
		virtual ~KMeanProcessorImpl();
	public:
		virtual void		SetK(g_uint k);

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

		RESULTCODE			SaveCentroidResult(KMean* kmean, const char* className, FeatureWorksapce* pWorkspace, g_uint srid);
		RESULTCODE			SaveClusterResult(KMean* kmean, const char* className, FeatureWorksapce* pWorkspace, g_uint srid);

	private:
		g_uint		m_k;
		std::string	m_in_source_name;
		std::string	m_in_class_name;

		std::string m_out_source_name;
		std::string m_out_class_name;

		KMean		m_kmean;
	};
}

#endif //__AUGE_KMEAN_PROCESSOR_H__
