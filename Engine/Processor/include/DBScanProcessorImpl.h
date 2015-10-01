#ifndef __AUGE_DBSCAN_PROCESSOR_H__
#define __AUGE_DBSCAN_PROCESSOR_H__

#include <string>
#include "AugeProcessor.h"
#include "DBScan.h"

namespace auge
{
	class FeatureWorkspace;

	class DBScanProcessorImpl : public DBScanProcessor
	{
	public:
		DBScanProcessorImpl();
		virtual ~DBScanProcessorImpl();
	public:

		virtual void		SetUser(g_uint user);

		virtual void		SetEsplon(float esplon);
		virtual void		SetMinPoints(g_uint minpts);

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

		//RESULTCODE			SaveCentroidResult(KMeans* kmean, const char* className, FeatureWorkspace* pWorkspace, g_uint srid);
		//RESULTCODE			SaveClusterResult(KMeans* kmean, const char* className, FeatureWorkspace* pWorkspace, g_uint srid);

	private:
		float		m_eps;
		g_uint		m_min_pts;
		std::string	m_in_source_name;
		std::string	m_in_class_name;

		std::string m_out_source_name;
		std::string m_out_class_name;

		DBScan		m_dbscan;

		g_uint		m_user;
	};
}

#endif //__AUGE_DBSCAN_PROCESSOR_H__
