#ifndef __AUGE_GRID_POINT_GENERATOR_IMPL_H__
#define __AUGE_GRID_POINT_GENERATOR_IMPL_H__

#include "AugeProcessor.h"
#include "AugeGeometry.h"

namespace auge
{
	class FeatureClass;
	class FeatureWorkspace;

	class GridPointGeneratorImpl : public GridPointGenerator
	{
	public:
		GridPointGeneratorImpl();
		virtual ~GridPointGeneratorImpl();
	public:

		virtual void		SetUser(g_uint user);
		virtual	void		SetOutputDataSource(const char* sourceName);
		virtual	void		SetOutputFeatureClass(const char* className);
		virtual void		SetOutputSRID(g_uint srid);

		virtual void		SetExtent(GEnvelope& extent);
		virtual void		SetCellSize(float cellSize);

		virtual RESULTCODE	Execute();
		virtual void		Release();

	private:
		const char*			GetInputSource();
		const char*			GetInputFatureClass();

		const char*			GetOutputSource();
		const char*			GetOutputFatureClass();

		float				GetCellSize();
		GEnvelope&			GetExtent();

	private:
		FeatureClass*		CreateOutputFeatureClass(const char* className, g_uint srid, FeatureWorkspace* poutWorkspace);
		RESULTCODE			Generate(FeatureClass* poutFeatureClass);

	private:
		std::string m_out_source_name;
		std::string m_out_class_name;

		g_uint		m_srid;

		GEnvelope	m_extent;
		float		m_cell_size;

		g_uint		m_user;

	};
}

#endif //__AUGE_GRID_POINT_GENERATOR_IMPL_H__
