#ifndef __AUGE_GEOMETRY_DEF_IMPL_H__
#define __AUGE_GEOMETRY_DEF_IMPL_H__

#include "AugeField.h"

namespace auge
{
	class GeometryDefImpl : public GeometryDef, virtual public GeometryDef_2
	{
	public:
		GeometryDefImpl();
		virtual ~GeometryDefImpl();
	public:
		virtual	int				 GetSRID() const;
		virtual	bool			 HasZ()	const;
		virtual	bool			 HasM()	const;
		virtual	bool			 IsDefault() const;
		virtual augeGeometryType GeometryType()	const;
		virtual bool			 GetExtent(GEnvelope& extent) const;

		virtual	void			 SetSRID(int nSRID);
		virtual	void			 SetZ(bool bHasZ);
		virtual	void			 SetM(bool bHasM);
		virtual	void			 SetDefault(bool bIsDefault);
		virtual void			 SetGeometryType(augeGeometryType type);	
		virtual bool			 SetExtent(const GEnvelope& extent);	

		virtual GeometryDef*	 GetGeometryDef();
		virtual GeometryDef_2*	 GetGeometryDef_2();

		virtual void			 Release();
	private:
		g_int				m_srid;
		g_uint				m_tag;	
		augeGeometryType	m_geometry_type;
		GEnvelope			m_extent;	
	};

}

#endif //__AUGE_GEOMETRY_DEF_IMPL_H__
