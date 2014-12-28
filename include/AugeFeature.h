 #ifndef __AUGE_FEATURE_H__
#define __AUGE_FEATURE_H__

#include "AugeCore.h"
#include "AugeData.h"
#include "AugeGeometry.h"

#ifdef WIN32
#	ifdef AUGE_FEATURE_EXPORTS
#		define AUGE_EATURE_API __declspec(dllexport)
#	else
#		define AUGE_EATURE_API __declspec(dllimport)
#	endif
#else
#	define AUGE_EATURE_API
#endif

namespace auge
{
	class DataEngine;
	class GConnection;
	class Workspace;
	class FeatureClass;
	class Feature;
	class FeatureCursor;
	class GField;
	class GFields;
	class FeatureInsertCommand;
	class GFilter;

	typedef enum augeCursorType
	{
		augeDynamicCursor	= 0,
		augeStaticCursor	= 1
	}augeCursorType;
	
	class FeatureWorksapce : public Workspace
	{
	protected:
		FeatureWorksapce(){}
		virtual ~FeatureWorksapce(){}
	public:
		virtual	FeatureClass*	OpenFeatureClass(const char* name) = 0;
		virtual RESULTCODE		CreateFeatureClass(const char* name, GFields* pFields) = 0;
		virtual RESULTCODE		RemoveFeatureClass(const char* name) = 0;

	};

	class FeatureClass : public GObject
	{
	protected:
		FeatureClass(){}
		virtual ~FeatureClass(){}
	public:
		virtual const char*			GetName() = 0;
		virtual g_uint				GetSRID() = 0;
		virtual GEnvelope&			GetExtent() = 0;
	
		virtual GFields*			GetFields() = 0;
		virtual GField*				GetField(const char* name) = 0;
		
		virtual FeatureCursor*		Query(augeCursorType type=augeStaticCursor) = 0;
		virtual FeatureCursor*		Query(GEnvelope& extent, augeCursorType type=augeStaticCursor) = 0;
		virtual FeatureCursor*		Query(GFilter* pFilter, augeCursorType type=augeStaticCursor) = 0;
		
		virtual FeatureInsertCommand* CreateInsertCommand() = 0;
	};

	class Feature : public GObject
	{
	protected:
		Feature(){}
		virtual ~Feature(){}
	public:

		virtual FeatureClass*		GetFeatureClass() const = 0;

		virtual g_int				GetFID() = 0;
		virtual Geometry*			GetGeometry() = 0;

		virtual GValue*				GetValue(g_uint i)		const = 0;
		virtual bool				GetBool(g_uint i)		const = 0;
		virtual char				GetChar(g_uint i)		const = 0;
		virtual short				GetShort(g_uint i)		const = 0;
		virtual int					GetInt(g_uint i)		const = 0;
		virtual long				GetLong(g_uint i)		const = 0;
		virtual float				GetFloat(g_uint i)		const = 0;
		virtual double				GetDouble(g_uint i)		const = 0;
		virtual int64				GetInt64(g_uint i)		const = 0;
		virtual const char*			GetString(g_uint i)		const = 0;
		//virtual const AgString*	GetString(g_uint i)		const = 0;
		//virtual const AgBlob*	GetBlob(g_uint i)		const = 0;
		//virtual const TIME_STRU*GetTime(g_uint i)		const = 0;

		virtual GValue*				GetValue(const char* name)		const = 0;
		virtual bool				GetBool(const char* name)		const = 0;
		virtual char				GetChar(const char* name)		const = 0;
		virtual short				GetShort(const char* name)		const = 0;
		virtual int					GetInt(const char* name)		const = 0;
		virtual long				GetLong(const char* name)		const = 0;
		virtual float				GetFloat(const char* name)		const = 0;
		virtual double				GetDouble(const char* name)		const = 0;
		virtual int64				GetInt64(const char* name)		const = 0;
		virtual const char*			GetString(const char* name)		const = 0;
		//virtual const AgString*	GetString(const char* name)		const = 0;
		//virtual const AgBlob*	GetBlob(const char* name)		const = 0;
		//virtual const TIME_STRU*GetTime(const char* name)		const = 0;
	};

	class FeatureCursor : public GObject
	{
	protected:
		FeatureCursor(){}
		virtual ~FeatureCursor(){}
	public:
		virtual Feature*		NextFeature() = 0;
		virtual FeatureClass*	GetFeatureClass() = 0;
	};

	class FeatureInsertCommand : public GObject
	{
	protected:
		FeatureInsertCommand(){}
		virtual ~FeatureInsertCommand(){}
	public:
		virtual	RESULTCODE	Prepare(FeatureClass* pFeatureClass) = 0;
		virtual RESULTCODE	Insert(Feature* pFeature) = 0;
	};
}

#endif //__AUGE_FEATURE_H__
