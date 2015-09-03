#ifndef __AUGE_IDW_FEATURE_PROCESSOR_IMPL_H__
#define __AUGE_IDW_FEATURE_PROCESSOR_IMPL_H__

#include "AugeProcessor.h"
#include "AugeGeometry.h"

namespace auge
{
	class FeatureClass;
	class FeatureWorkspace;

	class FeatureIDWProcessorImpl : public FeatureIDWProcessor
	{
	private:
		typedef struct  
		{
			double x;
			double y;
			double z;
		}idw_source_point_t;
	public:
		FeatureIDWProcessorImpl();
		virtual ~FeatureIDWProcessorImpl();
	public:

		virtual void		SetUser(g_uint user);

		virtual	void		SetInputDataSource(const char* sourceName);
		virtual	void		SetInputFeatureClass(const char* className);
		virtual void		SetInputZField(const char* fname);

		virtual	void		SetOutputDataSource(const char* sourceName);
		virtual	void		SetOutputFeatureClass(const char* className);
		virtual void		SetOutputZField(const char* fname);
		
		virtual void		SetExtent(GEnvelope& extent);
		virtual void		SetCellSize(double cellSize);

		virtual RESULTCODE	Execute();
		virtual void		Release();

	private:
		const char*			GetInputDataSource();
		const char*			GetInputFatureClass();
		const char*			GetInputZField();

		const char*			GetOutputDataSource();
		const char*			GetOutputFatureClass();
		const char*			GetOutputZField();
		
		double				GetCellSize();
		GEnvelope&			GetExtent();

	private:
		bool				Interpolate(FeatureClass* poutFeatureClass);
		bool				GenerateGrid();
		bool				ReadSourcePoints(FeatureClass* pinFeatureClass);
		FeatureClass*		CreateOutputFeatureClass(const char* className, const char* zfield, g_uint srid, FeatureWorkspace* poutWorkspace);
		double				IDW_Interplate(double x, double y);
	private:
		std::string	m_in_source_name;
		std::string	m_in_class_name;
		std::string m_in_z_field;

		std::string	m_out_source_name;
		std::string	m_out_class_name;
		std::string m_out_z_field;

		GEnvelope	m_extent;
		double		m_cell_size;

		g_uint		m_user;

		g_uint		m_rows;
		g_uint		m_cols;
		g_uint		m_point_count;
		idw_source_point_t* m_source_points;
	};

}

#endif //__AUGE_IDW_FEATURE_PROCESSOR_IMPL_H__
