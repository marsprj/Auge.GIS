#ifndef __AUGE_CORE_H__
#define __AUGE_CORE_H__

#ifdef WIN32
#	ifdef AUGE_EXPORTS
#		define AUGE_API __declspec(dllexport)
#	else
#		define AUGE_API __declspec(dllimport)
#	endif
#else
#	define AUGE_API
#endif

#include "AugeType.h"
#include "AugeObject.h"
#include <map>
#include <string>
#include <vector>

#ifdef __linux__
#	include <dlfcn.h>
#	include <unistd.h>
#endif

namespace auge
{
	enum
	{
		AlphaShift		= 24,
		RedShift		= 16,
		GreenShift		= 8,
		BlueShift		= 0
	};

	class Geometry;
	
	class AUGE_API GColor
	{
	public:
		GColor();
		explicit GColor(g_ulong color);
		GColor(g_byte r, g_byte g, g_byte b, g_byte a);
		virtual ~GColor();
	public:
		g_byte		GetAlpha();
		g_byte		GetRed();
		g_byte		GetGreen();
		g_byte		GetBlue();

		float		GetAlphaF();
		float		GetRedF();
		float		GetGreenF();
		float		GetBlueF();

		void		SetAlpha(g_byte a);
		void		SetRed(g_byte r);
		void		SetGreen(g_byte g);
		void		SetBlue(g_byte b);

		void		Set(g_ulong color);
		void		Set(g_byte r, g_byte g, g_byte b, g_byte a);
		void		Set(GColor &color);
	private:
		g_ulong		m_color;
		float		m_r, m_g, m_b, m_a;
	};

	class AUGE_API GSize
	{
	public:
		GSize()
		{
			m_width  = 0;
			m_height = 0;
		}
		GSize(g_uint w, g_uint h)
		{
			m_width  = w;
			m_height = h;
		}
		void Set(g_uint w, g_uint h)
		{
			m_width  = w;
			m_height = h;
		}
		g_uint Width()
		{
			return m_width;
		}
		g_uint Height()
		{
			return m_height;
		}
	private:
		g_uint	m_width;
		g_uint	m_height;
	};

	class AUGE_API GPropertySet
	{
	public:
		GPropertySet();
		GPropertySet(const GPropertySet& other);
		virtual ~GPropertySet();

	public:
		bool			Parse(const char* connection_string, const char *delimit=";");
		void			Clear();

		bool			IsEmpty() const;
		bool			SetValue(const char* key,const char* value);
		const char*		GetValue(const char* key) const;
	public:
		struct _Less
		{
			bool operator() (const std::string & s1, const std::string & s2) const
			{
				return g_stricmp(s1.c_str(), s2.c_str()) < 0;
			}
		};
	private:
		std::map<std::string, std::string> m_props;
	};

	class GBlob : public GObject
	{
	protected:
		GBlob(){}
		virtual ~GBlob(){}
	};

	class AUGE_API GValue : public GObject
	{
	public:
		GValue();
		GValue(augeValueType vt);
		GValue(bool    val);
		GValue(char    val);
		GValue(short   val);
		GValue(int     val);
		GValue(long    val);
		GValue(int64 val);
		GValue(float   val);
		GValue(double  val);
		GValue(const char* val);
		//GValue(AgString*   val,bool copy = false);
		//GValue(const char*   val,bool copy = false);
		GValue(GBlob*     val,bool copy = false);
		GValue(TIME_STRU* val,bool copy = false);
		GValue(Geometry*  val,bool copy = false);
		//GValue(const AgString*   val);
		GValue(const GBlob*     val);
		GValue(const TIME_STRU* val);
		GValue(const Geometry*  val);
		GValue(const GValue& other);
		GValue(const GValue* other);
		virtual ~GValue();
	public:
		augeValueType	GetType() const;
		void			Clear();
		bool			IsNull() const;
		void			SetType(augeValueType vt);
		bool			GetBool()		const;
		char			GetChar()		const;
		short			GetShort()		const;
		int				GetInt()		const;
		long			GetLong()		const;
		float			GetFloat()		const;
		double			GetDouble()		const;
		int64			GetInt64()		const;
		//const AgString*	GetString()		const;
		const char*	GetString()		const;
		const GBlob*	GetBlob()		const;
		const TIME_STRU*GetTime()		const;
		const Geometry*	GetGeometry()	const;
		//AgString*		GetString();
		const char*		GetString();
		GBlob*			GetBlob();
		TIME_STRU*		GetTime();
		Geometry*		GetGeometry();
		long			SetBool(bool val);
		long			SetChar(char val);
		long			SetShort(short val);
		long			SetInt(int val);
		long			SetLong(long val);
		long			SetFloat(float val);
		long			SetDouble(double val);
		long			SetInt64(int64 val);
		//long			SetString  (AgString*   val,bool copy = false);
		long			SetString  (const char* val,bool copy = false);
		long			SetBlob    (GBlob*     val,bool copy = false);
		long			SetTime    (TIME_STRU* val,bool copy = false);
		long			SetGeometry(Geometry*  val,bool copy = false);
		long			SetValue(const GValue& rValue);
		long			SetValue(const GValue* rValue);
		bool			Equals(const GValue* pValue) const;
		bool			Equals(const GValue& rValue) const;
		//AgString		ToString() const;

		void            SetModified(bool modified);
		bool            IsModified() const;
	public: // operator functions
		GValue& operator=(const GValue& other);
		GValue& operator=(bool val);
		GValue& operator=(char val);
		GValue& operator=(short val);
		GValue& operator=(int val);
		GValue& operator=(long val);
		GValue& operator=(int64 val);
		GValue& operator=(float val);
		GValue& operator=(double val);

		GValue& operator=(const char* val); //LiuDL

		//GValue& operator=(const AgString* val);
		GValue& operator=(const GBlob* val);
		GValue& operator=(const TIME_STRU* val);
		GValue& operator=(const Geometry* val);

		//GValue& operator=(AgString* val);
		GValue& operator=(GBlob* val);
		GValue& operator=(TIME_STRU* val);
		GValue& operator=(Geometry* val);

		// Literal comparison. Types and values must match.
		bool	operator==(const GValue& other) const;
		bool	operator!=(const GValue& other) const;
		bool	operator< (const GValue& other) const;
		bool	operator<=(const GValue& other) const;
		bool	operator> (const GValue& other) const;
		bool	operator>=(const GValue& other) const;

		operator char* () const;
		operator bool    () const;
		operator char    () const;
		operator int     () const;
		operator int64 () const;
		operator float   () const;
		operator double  () const;

	public: // Serialization functions
		long	SerializeSize() const;
		void	Serialize(char* pBytes,long lBytesSize) const;
		bool	Deserialize(const char* pBytes, long lByteSize);

		// 外部管理对象，此处不负责释放
		void	Release();

	private:
		typedef struct  tagVARIANT_STRU
		{
			short vt;
			union
			{
				char       chrVal;
				bool       blrVal;
				short      shtVal;
				int        intVal;
				long       lngVal;
				int64	   i64Val;

				float      fltVal;
				double     dblVal;

				char*	   strVal;
				GBlob*	   blbVal;
				TIME_STRU* timVal;
				Geometry*  gmtVal;
			};
			bool m_isModified;
		} VARIANT_STRU;
	private:
		void InitVariant(VARIANT_STRU& val);
	private:
		VARIANT_STRU  m_Variant;
	};

	class AUGE_API EnumValue
	{
	public:
		EnumValue();
		virtual ~EnumValue();
	public:
		virtual void	Reset();
		virtual	GValue*	Next();
		virtual void	Release();

	public:
		void	Add(GValue* pValue);
	private:
		void	Cleanup();

	private:
		std::vector<GValue*> m_values;
		std::vector<GValue*>::iterator m_iter;
	};

	class GError
	{
	protected:
		GError(){}
		virtual ~GError(){}
	public:
		virtual void		SetError(const char* szError) = 0;
		virtual const char*	GetLastError() = 0;
	};

	class GLogger
	{
	protected:
		GLogger(){}
		virtual ~GLogger(){}
	public:
		virtual void	Trace(const char* info, const char* fname=NULL, g_uint line=0) = 0;
		virtual void	Debug(const char* info, const char* fname=NULL, g_uint line=0) = 0;
		virtual void	Info (const char* info, const char* fname=NULL, g_uint line=0) = 0;
		virtual void	Warn (const char* info, const char* fname=NULL, g_uint line=0) = 0;
		virtual void	Error(const char* info, const char* fname=NULL, g_uint line=0) = 0;
		virtual void	Fatal(const char* info, const char* fname=NULL, g_uint line=0) = 0;

		virtual bool	Initialize(const char* config_path=NULL) = 0;
		virtual bool	IsInitialized() = 0;
	};

	extern "C"
	{
		AUGE_API GLogger*	augeGetLoggerInstance();
		AUGE_API GError*	augeGetErrorInstance();
	};

	extern "C"
	{
		AUGE_API g_int		auge_get_pid();
		AUGE_API void*		auge_malloc(size_t size, size_t count);
		AUGE_API void		auge_free(void *ptr);

		AUGE_API void		auge_get_cwd(char* path, size_t size);
		AUGE_API bool		auge_get_parent_dir(char* dir, char* pdir, g_uint size);
		AUGE_API void		auge_make_path(char* path, const char* drv, const char* dir, const char* file, const char* ext);
		AUGE_API void		auge_split_path( const char *path, char *drive, char *dir, char *fname, char *ext);
		AUGE_API g_int		auge_mkdir(const char* path);

		AUGE_API g_int		auge_find_last_char(const char *str, char ch);
		AUGE_API void		auge_str_upper(char* str);
		AUGE_API void		auge_str_lower(char* str);

		AUGE_API void		auge_generate_uuid(char* uuid, size_t size);
		AUGE_API g_int		auge_get_image_suffix(const char* mime_type, char* suffix, size_t size);
	
		AUGE_API void*		auge_load_library(const char* path);
		AUGE_API void		auge_free_library(void* handler);
		AUGE_API void*		auge_get_function_pointer(void* handler, const char* fname);

		AUGE_API bool		auge_unzip(const char* zip_path, const char* dest_path);
		AUGE_API bool		auge_zip(const char* src_path, const char* zip_path);

		AUGE_API g_ulong	auge_get_time();

		AUGE_API const char*	auge_encoding_convert(const char* from, const char* to, const char* instr, size_t instrlen);
		AUGE_API size_t	auge_encoding_convert_2(const char* from, const char* to, const char* instr, size_t instrlen, char* outstr, size_t* outlen);
	}
}

//==============================================================================
//------------------------------------------------------------------------------
// 连接所需动态库
//------------------------------------------------------------------------------
#ifdef WIN32
#	ifndef AUGE_EXPORTS
#		define _AUGE_CORE_NAME "GCore"
#		if defined(_DEBUG)
#			if defined(_UNICODE)
//#				define _AUGE_FILTER_CONFIGURATION "UD"
#				define _AUGE_FILTER_CONFIGURATION "D"
#			else
#				define _AUGE_FILTER_CONFIGURATION "D"
#			endif // _UNICODE
#		else
#			if defined(_UNICODE)
#				define _AUGE_FILTER_CONFIGURATION "U"
#			else
#				define _AUGE_FILTER_CONFIGURATION ""
#			endif // _UNICODE
#		endif // _DEBUG
#		pragma comment(lib, _AUGE_CORE_NAME _AUGE_FILTER_CONFIGURATION ".lib")
#		pragma message("Automatically linking with " _AUGE_CORE_NAME _AUGE_FILTER_CONFIGURATION ".dll")
#		undef _AUGE_FILTER_CONFIGURATION
#		undef _AUGE_CORE_NAME
#	endif // AUGE_FILTER_EXPORTS 
#endif
//==============================================================================


#endif //__AUGE_CORE_H__
