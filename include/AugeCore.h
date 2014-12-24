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

	class GValue : public GObject
	{
	protected:
		GValue(){}
		virtual ~GValue(){}
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

		AUGE_API void		auge_get_cwd(char* path, g_uint size);
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

		AUGE_API const char* auge_encoding_convert(const char* from, const char* to, const char* instr, size_t instrlen);
		AUGE_API size_t		auge_encoding_convert_2(const char* from, const char* to, const char* instr, size_t instrlen, char* outstr, size_t* outlen);
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
