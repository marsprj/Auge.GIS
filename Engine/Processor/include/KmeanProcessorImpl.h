#ifndef __AUGE_KMEAN_PROCESSOR_H__
#define __AUGE_KMEAN_PROCESSOR_H__

#include <string>
#include "AugeProcessor.h"
#include "Kmean.h"

namespace auge
{
	class FeatureWorkspace;

	class KMeansProcessorImpl : public KMeansProcessor
	{
	public:
		KMeansProcessorImpl();
		virtual ~KMeansProcessorImpl();
	public:

		virtual void		SetUser(g_uint user);

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

		RESULTCODE			SaveCentroidResult(KMeans* kmean, const char* className, FeatureWorkspace* pWorkspace, g_uint srid);
		RESULTCODE			SaveClusterResult(KMeans* kmean, const char* className, FeatureWorkspace* pWorkspace, g_uint srid);

	private:
		g_uint		m_k;
		std::string	m_in_source_name;
		std::string	m_in_class_name;

		std::string m_out_source_name;
		std::string m_out_class_name;

		KMeans		m_kmean;

		g_uint		m_user;
	};
}

#endif //__AUGE_KMEAN_PROCESSOR_H__
