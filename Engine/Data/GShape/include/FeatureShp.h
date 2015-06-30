#ifndef __AUGE_FEATURE_SHP_H__
#define __AUGE_FEATURE_SHP_H__

#include "AugeFeature.h"
#include "shapefil.h"

namespace auge
{
	class Geometry;
	class FeatureClassShp;

	class FeatureShp : public Feature
	{
	public:
		FeatureShp();
		virtual ~FeatureShp();

	public:
		virtual FeatureClass*	GetFeatureClass() const;
		virtual g_int			GetFID();
		virtual Geometry*		GetGeometry();
		virtual void			Release();

		virtual GValue*			GetValue(g_uint i)		const;
		virtual bool			GetBool(g_uint i)		const;
		virtual char			GetChar(g_uint i)		const;
		virtual short			GetShort(g_uint i)		const;
		virtual int				GetInt(g_uint i)		const;
		virtual long			GetLong(g_uint i)		const;
		virtual float			GetFloat(g_uint i)		const;
		virtual double			GetDouble(g_uint i)		const;
		virtual int64			GetInt64(g_uint i)		const;
		virtual const char*		GetString(g_uint i)		const;
		//virtual const AgString*	GetString(g_uint i)		const;
		//virtual const AgBlob*	GetBlob(g_uint i)		const;
		virtual TIME_STRU*		GetTime(g_uint i)		const;

		virtual GValue*			GetValue(const char* name)		const;
		virtual bool			GetBool(const char* name)		const;
		virtual char			GetChar(const char* name)		const;
		virtual short			GetShort(const char* name)		const;
		virtual int				GetInt(const char* name)		const;
		virtual long			GetLong(const char* name)		const;
		virtual float			GetFloat(const char* name)		const;
		virtual double			GetDouble(const char* name)		const;
		virtual int64			GetInt64(const char* name)		const;
		virtual const char*		GetString(const char* name)		const;
		//virtual const AgString*	GetString(const char* name)		const;
		//virtual const AgBlob*	GetBlob(const char* name)		const;
		//virtual const TIME_STRU*GetTime(const char* name)		const;

		virtual RESULTCODE			SetValue(const char* name, GValue *pValue);

	public:
		bool		Create(g_int lFID, FeatureClassShp* pFeatureClassShp);
		Geometry*	CreateGeometry(long index, SHPHandle pSHPHandle);

	private:
		FeatureClassShp	*m_pFeatureClass;
		GFields			*m_pFields;
		Geometry		*m_pGeometry;
		g_int			m_fid;

		SHPHandle		m_pSHPHandle;
		DBFHandle		m_pDBFHandle;
		
	};
}

#endif //__AUGE_FEATURE_SHP_H__
