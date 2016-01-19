#ifndef __AUGE_DELAUNAY_PROCESSOR_IMPL_H__
#define __AUGE_DELAUNAY_PROCESSOR_IMPL_H__

#include "AugeProcessor.h"
#include "AugeGeometry.h"
#include "Delaunay.h"

namespace auge
{
	class FeatureClass;
	class FeatureWorkspace;

	class DelaunayProcessorImpl : public DelaunayProcessor
	{
	private:
		typedef struct  
		{
			double	x;
			double	y;
			double	z;
		}d_vertext_t;

		typedef struct  
		{
			g_int	v1;
			g_int	v2;
		}d_edge_t;

		typedef struct
		{
			g_int	v1;
			g_int	v2;
			g_int	v3;
		}d_triangle_t;

	public:
		DelaunayProcessorImpl();
		virtual ~DelaunayProcessorImpl();
	public:

		virtual void		SetUser(g_uint user);

		virtual	void		SetInputDataSource(const char* sourceName);
		virtual	void		SetInputFeatureClass(const char* className);
		virtual void		SetInputZField(const char* fname);

		virtual	void		SetOutputDataSource(const char* sourceName);
		virtual	void		SetOutputFeatureClass(const char* className);
		virtual void		SetOutputZField(const char* fname);
		
		virtual RESULTCODE	Execute();
		virtual void		Release();

	private:
		const char*			GetInputDataSource();
		const char*			GetInputFatureClass();
		const char*			GetInputZField();

		const char*			GetOutputDataSource();
		const char*			GetOutputFatureClass();
		const char*			GetOutputZField();
		
	private:
		bool				LoadVertex(FeatureClass* pinFeatureClass, delaunay_node_t** vertexes, g_uint& vertex_count);
		bool				WriteEdges(Delaunay& delaunay, const char* outClassName, FeatureWorkspace* poutWorkspace, g_uint srid);
		FeatureClass*		CreateEdgeFeatureClass(const char* outClassName, FeatureWorkspace* poutWorkspace, g_uint srid);

		bool				WriteTriangles(Delaunay& delaunay, const char* outClassName, FeatureWorkspace* poutWorkspace, g_uint srid);
		FeatureClass*		CreateTriangleFeatureClass(const char* outClassName, FeatureWorkspace* poutWorkspace, g_uint srid);
		
	private:
		std::string	m_in_source_name;
		std::string	m_in_class_name;
		std::string m_in_z_field;

		std::string	m_out_source_name;
		std::string	m_out_class_name;
		std::string m_out_z_field;
		
		g_uint		m_user;
	};

}

#endif //__AUGE_DELAUNAY_PROCESSOR_IMPL_H__
