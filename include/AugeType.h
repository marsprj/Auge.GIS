#ifndef __AUGE_TYPE_H__
#define __AUGE_TYPE_H__

typedef char				g_char;
typedef unsigned char		g_uchar;
typedef char				g_int8;
typedef unsigned char		g_uint8;
typedef unsigned char		g_byte;
typedef short				g_int16;
typedef unsigned short		g_uint16;
typedef int					g_int;
typedef unsigned int		g_uint;
typedef unsigned long		g_ulong;

#ifndef PI
#define PI					3.14159265358979323846
#endif

#define TWO_PI				(PI*2.0) 
#define HALF_PI				(PI*0.5)
//#define MAXLONG				2147483647
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

#define	AUGE_PATH_MAX	1024
#define	AUGE_NAME_MAX	256
#define	AUGE_EXT_MAX	8
#define	AUGE_MSG_MAX	256
#define	AUGE_MSG_LONG_MAX	1024
#define AUGE_BUFFER_MAX	4096
#define AUGE_SQL_MAX	1024
#define AUGE_SQL_LONG_MAX	8192

#ifndef PATH_MAX
#	define PATH_MAX 1024
#endif

#ifndef NAME_MAX
#	define NAME_MAX	256
#endif


#define AUGE_DB_SERVER		"SERVER"
#define AUGE_DB_INSTANCE	"INSTANCE"
#define AUGE_DB_DATABASE	"DATABASE"
#define AUGE_DB_USER		"USER"
#define AUGE_DB_PASSWORD	"PASSWORD"
#define AUGE_DB_ENCODING	"ENCODING"

#ifndef g_stricmp
#	ifdef WIN32
#		define g_stricmp	_stricmp
#		define g_strnicmp	_strnicmp
#		define g_sprintf	sprintf
#		define g_snprintf	_snprintf
#		define g_strncpy	strncpy
#	else
#		define g_stricmp	strcasecmp
#		define g_strnicmp	strncasecmp
#		define g_sprintf	sprintf
#		define g_snprintf	snprintf
#		define g_strncpy	strncpy
#	endif
#endif

#ifdef WIN32
#	define	g_access	_access
#else
#	define	g_access	access
#endif

#define	RESULTCODE	g_int
#define AG_SUCCESS	 0
#define AG_FAILURE	-1

typedef enum augeUnitType
{
	augeUnknownUnits		= 0,
	augeInches				= 1,
	augePoints				= 2,
	augeFeet				= 3,
	augeYards				= 4,
	augeMiles				= 5,
	augeNauticalMiles		= 6,
	augeMillimeters			= 7,
	augeCentimeters			= 8,
	augeMeters				= 9,
	augeKilometers			= 10,
	augeDecimalDegrees		= 11,
	augeDecimeters			= 12,
	augeUnitsLast			= 13
}
augeUnitType;

typedef enum augeFieldType
{
	augeFieldTypeNone     = 0,
	augeFieldTypeBool     = 1,
	augeFieldTypeChar     = 2,
	augeFieldTypeShort    = 3,
	augeFieldTypeInt      = 4,
	augeFieldTypeLong     = 5,
	augeFieldTypeInt64    = 6,
	augeFieldTypeFloat    = 7,
	augeFieldTypeDouble   = 8,
	augeFieldTypeString   = 9,
	augeFieldTypeTime     = 10,
	augeFieldTypeBLOB     = 11,
	augeFieldTypeGeometry = 12,
	augeFieldTypeTopoGeometry = 13,
	augeFieldTypeCLOB	  = 14,
	augeFieldTypeNCLOB	  = 15,
	augeFieldTypeSerial	  = 16

}augeFieldType;
//==============================================================================

#define AUGE_SAFE_RELEASE(p) {if((p)!=NULL){(p)->Release(); (p)=NULL;}} 

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <strings.h>
#include <limits.h>
#ifdef WIN32
#	include <windows.h>
#	include <io.h>
#else
#	include "unistd.h"
#	include "sys/types.h"
#	include "dirent.h"
#endif

#endif //__AUGE_TYPE_H__