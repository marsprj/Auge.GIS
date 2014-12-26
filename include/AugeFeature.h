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
	};

	class FeatureClass : public GObject
	{
	protected:
		FeatureClass(){}
		virtual ~FeatureClass(){}
	public:
		virtual const char*			GetName() = 0;
		virtual GFields*			GetFields() = 0;
		virtual GField*				GetField(const char* name) = 0;
		
		virtual FeatureCursor*		Query(augeCursorType type=augeStaticCursor) = 0;
	};

	class Feature : public GObject
	{
	protected:
		Feature(){}
		virtual ~Feature(){}
	public:
		virtual g_int				GetFID() = 0;
		virtual Geometry*			GetGeometry() = 0;
	};

	class FeatureCursor : public GObject
	{
	protected:
		FeatureCursor(){}
		virtual ~FeatureCursor(){}
	public:
		virtual Feature*	NextFeature() = 0;
	};

}

#endif //__AUGE_FEATURE_H__
