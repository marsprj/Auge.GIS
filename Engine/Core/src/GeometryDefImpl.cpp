#include "GeometryDefImpl.h"

namespace auge
{

	// ------------------------------------------------------------------- augeGeometryDefTag
	typedef enum augeGeometryDefTag
	{
		augeGeometryDefNone			= 0x0000,
		augeGeometryDefHasZ			= 0x0001,
		augeGeometryDefHasM			= 0x0002,
		augeGeometryDefDefault		= 0x0004,
		augeGeometryDefR1			= 0x0008,
		augeGeometryDefR2			= 0x0010,
		augeGeometryDefR3			= 0x0020,
		augeGeometryDefR4			= 0x0040,
		augeGeometryDefR5			= 0x0080
	} augeGeometryDefTag;

	// ------------------------------------------------------------------- Tag Operation Macro
#define		HasTagX(tags,tagx)	(((tags&tagx)==tagx))
#define		CnlTagX(tags,tagx)	(tags=((tags&(~tagx))))
#define		SetTagX(tags,tagx)	(tags=((tags|tagx)))

	GeometryDefImpl::GeometryDefImpl():
	m_srid(4326),
		m_tag(augeGeometryDefNone),
		m_geometry_type(augeGTNull)
	{

	}

	GeometryDefImpl::~GeometryDefImpl()
	{

	}

	//////////////////////////////////////////////////////////////////////////

	int GeometryDefImpl::GetSRID() const
	{
		return m_srid;
	}

	bool GeometryDefImpl::HasZ()	const
	{
		return HasTagX(m_tag,augeGeometryDefHasZ);
	}

	bool GeometryDefImpl::HasM()	const
	{
		return HasTagX(m_tag,augeGeometryDefHasM);
	}

	bool GeometryDefImpl::IsDefault() const
	{
		return true;
	}

	augeGeometryType GeometryDefImpl::GeometryType() const
	{
		return m_geometry_type;
	}

	bool GeometryDefImpl::GetExtent(GEnvelope& extent) const
	{
		return HasTagX(m_tag,augeGeometryDefDefault);
	}

	void GeometryDefImpl::SetSRID(int srid)
	{
		m_srid = srid;
	}

	void GeometryDefImpl::SetZ(bool bHasZ)
	{
		if (bHasZ)
		{
			SetTagX(m_tag,augeGeometryDefHasZ);
		}
		else
		{
			CnlTagX(m_tag,augeGeometryDefHasZ);
		}
	}

	void GeometryDefImpl::SetM(bool bHasM)
	{
		if (bHasM)
		{
			SetTagX(m_tag,augeGeometryDefHasM);
		}
		else
		{
			CnlTagX(m_tag,augeGeometryDefHasM);
		}
	}

	void GeometryDefImpl::SetDefault(bool bIsDefault)
	{
		if (bIsDefault)
		{
			SetTagX(m_tag,augeGeometryDefDefault);
		}
		else
		{
			CnlTagX(m_tag,augeGeometryDefDefault);
		}
	}

	void GeometryDefImpl::SetGeometryType(augeGeometryType type)
	{
		m_geometry_type= type;
	}

	bool GeometryDefImpl::SetExtent(const GEnvelope& extent)
	{
		m_extent = extent;
		return true;
	}

	GeometryDef* GeometryDefImpl::GetGeometryDef()
	{
		return (GeometryDef*)this;
	}

	GeometryDef_2* GeometryDefImpl::GetGeometryDef_2()
	{
		return (GeometryDef_2*)this;
	}

	void GeometryDefImpl::Release()
	{
		delete this;
	}

}
