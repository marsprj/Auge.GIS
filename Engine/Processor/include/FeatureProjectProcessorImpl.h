#ifndef __AUGE_FEATURE_PROJECT_PROCESSOR_H__
#define __AUGE_FEATURE_PROJECT_PROCESSOR_H__

#include <string>
#include "AugeProcessor.h"
#include "Kmean.h"

#include "proj_api.h"
#include "SrsBase.h"

namespace auge
{
	class FeatureClass;
	class FeatureWorkspace;

	class FeatureProjectProcessorImpl : public FeatureProjectProcessor
	{
	public:
		FeatureProjectProcessorImpl();
		virtual ~FeatureProjectProcessorImpl();
	public:

		virtual void		SetUser(g_uint user);

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

		FeatureClass*		CreateOutputFeatureClass(const char* className, g_uint srid, FeatureWorkspace* pWorkspace, FeatureClass* pinFeatureClass);
		RESULTCODE			Project_To_WGS84(FeatureClass* pinFeatureClass, FeatureClass* poutFeatureClass);
		RESULTCODE			Project_From_WGS84(FeatureClass* pinFeatureClass, FeatureClass* poutFeatureClass);

	private:
		RESULTCODE			Project_To_WGS84(WKBPoint* pWKBPoint, projPJ i_pj, projPJ o_pj);
		RESULTCODE			Project_To_WGS84(WKBMultiPoint* pWKBMultiPoint, projPJ i_pj, projPJ o_pj);
		RESULTCODE			Project_To_WGS84(WKBLineString* pWKBLineString, projPJ i_pj, projPJ o_pj);
		RESULTCODE			Project_To_WGS84(WKBMultiLineString* pWKBMultiLineString, projPJ i_pj, projPJ o_pj);
		RESULTCODE			Project_To_WGS84(WKBPolygon* pWKBPolygon, projPJ i_pj, projPJ o_pj);
		RESULTCODE			Project_To_WGS84(WKBMultiPolygon* pWKBMultiPolygon, projPJ i_pj, projPJ o_pj);

		RESULTCODE			Project_From_WGS84(WKBPoint* pWKBPoint, projPJ i_pj, projPJ o_pj);
		RESULTCODE			Project_From_WGS84(WKBMultiPoint* pWKBMultiPoint, projPJ i_pj, projPJ o_pj);
		RESULTCODE			Project_From_WGS84(WKBLineString* pWKBLineString, projPJ i_pj, projPJ o_pj);
		RESULTCODE			Project_From_WGS84(WKBMultiLineString* pWKBMultiLineString, projPJ i_pj, projPJ o_pj);
		RESULTCODE			Project_From_WGS84(WKBPolygon* pWKBPolygon, projPJ i_pj, projPJ o_pj);
		RESULTCODE			Project_From_WGS84(WKBMultiPolygon* pWKBMultiPolygon, projPJ i_pj, projPJ o_pj);

	private:
		std::string	m_in_source_name;
		std::string	m_in_class_name;

		std::string m_out_source_name;
		std::string m_out_class_name;

		g_uint		m_srid;
		SrsBase		m_srsbase;

		g_uint		m_user;
	};
}

#endif //__AUGE_FEATURE_PROJECT_PROCESSOR_H__
