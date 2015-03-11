#ifndef __AUGE_FEATURE_OBJ_H__
#define __AUGE_FEATURE_OBJ_H__

#include "AugeFeature.h"
#include <string>
#include <map>

namespace auge
{
	class GValue;
	class Geometry;
	class FeatureClassPgs;

	class FeatureObj : public Feature
	{
	public:
		FeatureObj(FeatureClassPgs* pFeatureClass);
		virtual ~FeatureObj();

	public:
		virtual FeatureClass*	GetFeatureClass() const;
		virtual g_int			GetFID();
		virtual Geometry*		GetGeometry();
		virtual void			Release();

		virtual GValue*			GetValue(g_uint i)			const;
		virtual bool			GetBool(g_uint i)		const;
		virtual char			GetChar(g_uint i)		const;
		virtual short			GetShort(g_uint i)		const;
		virtual int				GetInt(g_uint i)			const;
		virtual long			GetLong(g_uint i)		const;
		virtual float			GetFloat(g_uint i)		const;
		virtual double			GetDouble(g_uint i)		const;
		virtual int64			GetInt64(g_uint i)		const;
		virtual const char*		GetString(g_uint i)		const;

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

		virtual RESULTCODE		SetValue(const char* name, GValue *pValue);
		
	private:
		void		Cleanup();

	public:
		struct _Less
		{
			bool operator() (const std::string & s1, const std::string & s2) const
			{
				return g_stricmp(s1.c_str(), s2.c_str()) < 0;
			}
		};
	private:
		GFields			*m_pFields;
		FeatureClassPgs	*m_pFeatureClass;
		std::map<std::string, GValue*>	m_values;
	};
}

#endif //__AUGE_FEATURE_OBJ_H__
