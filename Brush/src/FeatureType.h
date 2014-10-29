#ifndef __AUGE_FEATURE_TYPE_H__
#define __AUGE_FEATURE_TYPE_H__

#include "Base.h"
#include "Envelope.h"
#include "Geometry.h"

#include <string>
#include <vector>

namespace auge
{
	class Field;
	class Connection;
	class FeatureSet;

	class AUGE_API FeatureType : public Base
	{
		
	public:
		FeatureType(Connection* pWorkspace);
		virtual ~FeatureType();
		
	public:
		const char*	GetName();
		int			SetName(const char* szName);

		Envelope&	GetExtent();

		int			SetGeometryColumn(const char* szGeometryColumn);
		int			SetGeometryType(const char* szType);
		void		SetDimension(int dimension);
		void		SetSrid(int srid);

		Field&		GetField(uint i);
		int			GetFieldIndex(const char* szName);
		int			GetGeometryFieldIndex();
		std::vector<Field*>& GetFields();
		

		FeatureSet*	Query(GeometryFormat format=WKB);
		FeatureSet*	Query(Envelope& extent, GeometryFormat format=WKB);

	private:
		int			BuildQuerySQL(char* sql, uint size, GeometryFormat format);
		int			BuildQuerySQL(char* sql, uint size, Envelope& extent, GeometryFormat format);

		void		CleanupFields();

	private:
		std::string	 m_name;
		std::string	 m_geometry_column;
		int			 m_dimension;
		int			 m_srid;
		GeometryType m_geom_type;
		Envelope	 m_extent;
		Connection*	 m_pWorkspace;

		std::vector<Field*>	m_fields;
	};

}//namespace

#endif //__AUGE_FEATURE_TYPE_H__
