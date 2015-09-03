#ifndef __AUGE_IDW_PROCESSOR_IMPL_H__
#define __AUGE_IDW_PROCESSOR_IMPL_H__

#include "AugeProcessor.h"
#include "AugeGeometry.h"

namespace auge
{
	class FeatureClass;
	class Raster;

	class IDWProcessorImpl : public IDWProcessor
	{
	private:
		typedef struct  
		{
			float x;
			float y;
			float z;
			float w;
		}idw_vertex_t;
	public:
		IDWProcessorImpl();
		virtual ~IDWProcessorImpl();
	public:

		virtual void		SetUser(g_uint user);

		virtual	void		SetInputDataSource(const char* sourceName);
		virtual	void		SetInputFeatureClass(const char* className);

		virtual	void		SetOutputDataSource(const char* sourceName);
		virtual void		SetOutputRaster(const char* rasterName);
		virtual void		SetOutputPath(const char* rasterPath);

		virtual void		SetZField(const char* fname);
		virtual void		SetExtent(GEnvelope& extent);
		virtual void		SetCellSize(float cellSize);

		virtual RESULTCODE	Execute();
		virtual void		Release();

	private:
		const char*			GetInputDataSource();
		const char*			GetInputFatureClass();

		const char*			GetOutputDataSource();
		const char*			GetOutputRaster();
		const char*			GetOutputRasterPath();

		const char*			GetZField();
		float				GetCellSize();
		GEnvelope&			GetExtent();

	private:
		Raster*				Interpolate(FeatureClass* pinFeatureClass);
		bool				GenerateGrid();
		
	private:
		std::string	m_in_source_name;
		std::string	m_in_class_name;

		std::string	m_out_source_name;
		std::string	m_out_raster_name;
		std::string m_out_raster_path;

		std::string m_z_field;
		GEnvelope	m_extent;
		float		m_cell_size;

		g_uint		m_user;

		g_uint		m_rows;
		g_uint		m_cols;
		g_uint		m_vertex_count;
		idw_vertex_t* m_vertexes;
	};

}

#endif //__AUGE_IDW_PROCESSOR_IMPL_H__
