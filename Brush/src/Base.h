#ifndef _AUGE_BASE_H__
#define _AUGE_BASE_H__

using namespace std;

typedef unsigned char		uchar;
typedef char				int8;
typedef unsigned char		uint8;
typedef unsigned char		byte;
typedef short				int16;
typedef unsigned short		uint16;
typedef int					int32;
typedef unsigned int		uint;
typedef unsigned long		ulong;

#ifndef PI
#define PI					3.14159265358979323846
#endif

#define TWO_PI				(PI*2.0) 
#define HALF_PI				(PI*0.5)
#define MAXLONG				2147483647
#define DBLLONG				4.61168601e18
#define EPSLN				1.0e-6
#define S2R					4.848136811095359e-6

#define D2R					(PI/ 180.0f)
#define R2D					(180/PI)

#define DEG2RAD(deg)		(deg*PI/180.0)
#define RAD2DEG(rad)		(rad*180.0/PI)

#ifndef FLOAT_MIN
#define FLOAT_MIN			(-3.402823E+38f)
#define FLOAT_MAX           (+3.402823E+38f)
#define FLOAT_ZERO          (+1.401298E-45f)
#endif

#ifndef DOUBLE_MIN
#define DOUBLE_MIN          (-1.7976931348623158e+308)
#define DOUBLE_MAX          (+1.7976931348623158e+308)
#define DOUBLE_ZERO         (+2.2250738585072014e-308)
#endif

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

#ifndef ag_stricmp
#	ifdef WIN32
#		define ag_stricmp	_stricmp
#		define ag_strnicmp	_strnicmp
#		define ag_snprintf	_snprintf
#	else
#		define ag_stricmp	strcasecmp
#		define ag_strnicmp	strncasecmp
#		define ag_snprintf	snprintf
#	endif
#endif

#define AG_SUCCESS	 0
#define AG_FAILURE	-1

#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef signed __int64          int64;
#else
typedef signed long long        int64;
#endif

#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef unsigned __int64        uint64;
#else
typedef unsigned long long      uint64;
#endif

#ifdef WIN32
#	ifdef MAP_BRUSH_EXPORTS
#		define AUGE_API __declspec(dllexport)
#	else
#		define AUGE_API __declspec(dllimport)
#	endif
#else
#	define AUGE_API
#endif

#include <stdlib.h>
#include <string.h>

namespace auge
{
	class AUGE_API Base
	{
	public:
		Base()
		{
			m_ref = 1;
			m_id = -1;
		}

		virtual ~Base(){}

		virtual int AddRef()
		{
			return (++m_ref);
		}

		virtual void Release()
		{
			if((--m_ref)==0)
			{
				delete this;
			}
		}

		int GetID()
		{
			return m_id;
		}

		void SetID(int id)
		{
			m_id = id;
		}

	private:
		int  m_id;
		int	 m_ref;
	};
}

#endif //_AUGE_BASE_H__
