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
typedef int					g_int32;
typedef unsigned int		g_uint;
typedef long				g_long;
typedef unsigned long		g_ulong;
#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef signed __int64      g_int64;
#else
typedef signed long int    g_int64;
#endif

#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef unsigned __int64    g_uint64;
#else
typedef unsigned long int  g_uint64;
#endif

#ifndef PI
#define PI					3.14159265358979323846
#endif

#define TWO_PI				(PI*2.0) 
#define HALF_PI				(PI*0.5)
//#define MAXLONG				2147483647
#define DBLLONG				4.61168601e18
#define AUGE_EPSILON		1.0e-6
#define S2R					4.848136811095359e-6

#define D2R					(PI/ 180.0f)
#define R2D					(180/PI)

#define DEG2RAD(deg)		(deg*PI/180.0)
#define RAD2DEG(rad)		(rad*180.0/PI)

#ifndef FLOAT_MIN
#define AUGE_FLOAT_MIN			(-3.402823E+38f)
#define AUGE_FLOAT_MAX           (+3.402823E+38f)
#endif

#define	AUGE_BYTE_MAX		255
#define	AUGE_BYTE_MIN		-255

#define AUGE_SHORT_MIN		(-32768)
#define AUGE_SHORT_MAX      32767
#define AUGE_USHORT_MAX     0xffff
#define AUGE_INT_MIN		(-2147483647 - 1)
#define AUGE_INT_MAX		2147483647
#define AUGE_UINT_MAX		0xffffffff

#ifndef AUGE_INT_MIN
#define AUGE_INT_MIN		(-16384)
#define AUGE_INT_MAX        (+16384)
#endif

#ifndef AUGE_DOUBLE_MIN
#define AUGE_DOUBLE_MIN          (-1.7976931348623158e+308)
#define AUGE_DOUBLE_MAX          (+1.7976931348623158e+308)
#define AUGE_DOUBLE_ZERO         (+2.2250738585072014e-308)
#endif

#define AUGE_RADIAN_TO_DEGREE	57.295779513082323
#define AUGE_DEGREE_TO_RADIAN	0.0174532925199433

#ifndef AUGE_VIRTUAL_NAME
#	define AUGE_VIRTUAL_NAME	"ows"
#endif

#define AUGE_TRUE			1
#define AUGE_FALSE			0

#define	AUGE_PATH_MAX		1024
#define	AUGE_NAME_MAX		256
#define	AUGE_EXT_MAX		8
#define	AUGE_DRV_MAX		8
#define	AUGE_MSG_MAX		256
#define	AUGE_MSG_LONG_MAX	1024
#define AUGE_BUFFER_MAX		4096
#define AUGE_SQL_MAX		1024
#define AUGE_SQL_LONG_MAX	8192
#define AUGE_ICON_SIZE		32
#define AUGE_ICON_SCALE		0.7

#define AUGE_THUMBNAIL_SIZE		256
#define AUGE_THUMBNAIL_WIDTH	512
#define AUGE_THUMBNAIL_HEIGHT	384

#define AUGE_EARTH_RADIUS	6378137	//meter
#define AUGE_PIXEL_WIDTH	0.028	//cm

#ifndef PATH_MAX
#	define PATH_MAX 1024
#endif

#ifndef NAME_MAX
#	define NAME_MAX	256
#endif

#ifdef WIN32
#define S_ISREG(m) (((m) & 0170000) == (0100000))   
#define S_ISDIR(m) (((m) & 0170000) == (0040000))  
#endif

#define AUGE_DB_SERVER		"SERVER"
#define AUGE_DB_INSTANCE	"INSTANCE"
#define AUGE_DB_DATABASE	"DATABASE"
#define AUGE_DB_USER		"USER"
#define AUGE_DB_PASSWORD	"PASSWORD"
#define AUGE_DB_ENCODING	"ENCODING"
#define AUGE_DB_REPOSITORY	"REPOSITORY"

#define AUGE_DEFAULT_ENCODING	"GBK"
#define AUGE_DEFAULT_SRID		4326
#define AUGE_DEFAULT_GEOM_FIELD "shape"

#define AUGE_SRID_WGS84			4326
#define AUGE_SRID_WEBMERCATOR	900913

#define AUGE_ENCODING_GBK	"GBK"
#define AUGE_ENCODING_UTF8	"UTF-8"

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

#define g_max(a,b)    (((a) > (b)) ? (a) : (b))
#define g_min(a,b)    (((a) < (b)) ? (a) : (b))

#define	RESULTCODE	g_int
#define AG_SUCCESS	 0
#define AG_FAILURE	-1

#ifndef TRUE
#	define	TRUE	1
#	define	FALSE	0
#endif

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

typedef enum augeValueType_e
{
	augeValueTypeNone			= 0,
	augeValueTypeBool			= 1,
	augeValueTypeChar			= 2,
	augeValueTypeShort			= 3,
	augeValueTypeInt			= 4,
	augeValueTypeLong			= 5,
	augeValueTypeInt64			= 6,
	augeValueTypeFloat			= 7,
	augeValueTypeDouble			= 8,
	augeValueTypeString			= 9,
	augeValueTypeTime			= 10,
	augeValueTypeBLOB			= 11,
	augeValueTypeGeometry		= 12,
	augeValueTypeNull			= 13,
} augeValueType;

typedef struct  tagTIME_STRU
{
	unsigned short usYear;
	unsigned short usMonth;
	unsigned short usDayOfWeek;
	unsigned short usDay;
	unsigned short usHour;
	unsigned short usMinute;
	unsigned short usSecond;
	unsigned short usMilliseconds;
} TIME_STRU;

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
#	include <unistd.h>
#	include <sys/types.h>
#	include <sys/stat.h> 
#	include <dirent.h>
#endif

#endif //__AUGE_TYPE_H__
