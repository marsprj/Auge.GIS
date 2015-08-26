#ifndef __AUGE_FEATURE_RAMDOM_TO_POINTS_PROCESSOR_H__
#define __AUGE_FEATURE_RAMDOM_TO_POINTS_PROCESSOR_H__

#include <string>
#include "AugeProcessor.h"
#include "AugeGeometry.h"

namespace auge
{
	class Feature;
	class FeatureClass;
	class FeatureWorkspace;
	class FeatureInsertCommand;

	class RadomPointsGeneratorImpl : public RadomPointsGenerator
	{
	public:
		RadomPointsGeneratorImpl();
		virtual ~RadomPointsGeneratorImpl();
	public:

		virtual void		SetUser(g_uint user);
		virtual	void		SetOutputDataSource(const char* sourceName);
		virtual	void		SetOutputFeatureClass(const char* className);
		virtual void		SetSRID(g_uint srid);
		virtual void		SetExtent(GEnvelope& extent);
		virtual void		SetCount(g_uint count);

		virtual RESULTCODE	Execute();
		virtual void		Release();

	private:
		const char*			GetOutputSource();
		const char*			GetOutputFatureClass();

		g_uint				GetSRID();
		g_uint				GetCount();
		GEnvelope&			GetExtent();

	private:
		FeatureClass*		CreateOutputFeatureClass(const char* className, FeatureWorkspace* pWorkspace);
		RESULTCODE			Process(FeatureClass* poutFeatureClass);
		
	private:
		std::string m_out_source_name;
		std::string m_out_class_name;
		std::string m_geom_field;
		g_uint		m_srid;
		g_uint		m_count;
		GEnvelope	m_extent;

		g_uint		m_user;
	};
}

#endif //__AUGE_FEATURE_RAMDOM_TO_POINTS_PROCESSOR_H__
