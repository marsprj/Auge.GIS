#ifndef __AUGE_FEATURE_RANDOM_POINTS_IN_POLYGON_PROCESSOR_H__
#define __AUGE_FEATURE_RANDOM_POINTS_IN_POLYGON_PROCESSOR_H__

#include <string>
#include "AugeProcessor.h"
#include "AugeGeometry.h"

namespace auge
{
	class Feature;
	class FeatureClass;
	class FeatureWorkspace;
	class FeatureInsertCommand;

	class RandomPointsInPolygonGeneratorImpl : public RandomPointsInPolygonGenerator
	{
	public:
		RandomPointsInPolygonGeneratorImpl();
		virtual ~RandomPointsInPolygonGeneratorImpl();
	public:

		virtual void		SetUser(g_uint user);
		virtual	void		SetInputDataSource(const char* sourceName);
		virtual	void		SetInputFeatureClass(const char* className);
		virtual	void		SetOutputDataSource(const char* sourceName);
		virtual	void		SetOutputFeatureClass(const char* className);
		virtual void		SetCount(g_uint count);

		virtual RESULTCODE	Execute();
		virtual GMultiPoint* Execute(Geometry* pGeometry, g_uint count);
		virtual void		Release();

	private:
		const char*			GetInputSource();
		const char*			GetInputFatureClass();

		const char*			GetOutputSource();
		const char*			GetOutputFatureClass();

		g_uint				GetSRID();
		g_uint				GetCount();
		GEnvelope&			GetExtent();

	private:
		FeatureClass*		CreateOutputFeatureClass(const char* className, FeatureWorkspace* pWorkspace, FeatureClass* pinFeatureClass);
		RESULTCODE			Process(FeatureClass* poutFeatureClass, FeatureClass* pinFeatureClass);
		void				GenerateRandomPoints(g_uint count, Geometry* pPolygon, FeatureClass* poutFeatureClass, FeatureInsertCommand* cmd);
		
	private:
		std::string	m_in_source_name;
		std::string	m_in_class_name;

		std::string m_out_source_name;
		std::string m_out_class_name;

		std::string m_geom_field;
		g_uint		m_srid;
		g_uint		m_count;
		GEnvelope	m_extent;

		g_uint		m_user;
	};
}

#endif //__AUGE_FEATURE_RANDOM_POINTS_IN_POLYGON_PROCESSOR_H__
