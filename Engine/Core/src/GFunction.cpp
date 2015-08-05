#include "AugeCore.h"
#include <iconv.h>
#include <math.h>
#ifdef WIN32
#	include <direct.h>
#endif

#ifdef WIN32
#	include "process.h"
#else
#	include <sys/types.h>
#	include <unistd.h>
#	include <time.h>
#	include <uuid/uuid.h>
#endif

namespace auge
{
#ifndef WIN32
	void _splitpath( const char *path, char *drive, char *dir, char *fname, char *ext);
	void _makepath( char *path, const char *drive, const char *dir, const char *fname, const char *ext);
#endif

	g_int auge_get_pid()
	{
#ifdef WIN32
		return _getpid();
#else
		return getpid();
#endif
	}

	void* auge_malloc(size_t size, size_t count)
	{
		return malloc(size*count);
	}

	void auge_free(void* ptr)
	{
		free(ptr);
	}

	void auge_get_cwd(char* szpath, size_t length)
	{
#ifdef WIN32
		char module_path[_MAX_PATH];
		GetModuleFileName(NULL, module_path, _MAX_PATH);
		char drv[_MAX_DRIVE];
		char dir[_MAX_DIR];
		_splitpath(module_path, drv, dir, NULL, NULL);
		_makepath(szpath, drv, dir, NULL, NULL);
#else
		getcwd(szpath, length);
#endif
	}

	void auge_make_path(char* path, const char* drv, const char* dir, const char* file, const char* ext)
	{
		_makepath(path, drv, dir, file, ext);

		
		for(char* ptr=path; *ptr!='\0'; ptr++)
		{
			if(*ptr == '\\')
			{
				*(ptr) = '/';
			}
		}
	}

	void auge_split_path( const char *path, char *drive, char *dir, char *fname, char *ext)
	{
		_splitpath(path, drive, dir, fname, ext);
	}

	bool auge_get_parent_dir(char* dir, char* pdir, g_uint size)
	{
		if(dir==NULL)
		{
			return false;
		}

		g_int pos = auge_find_last_char(dir, '\\');
		if(pos>0)
		{
			memset(pdir, 0, size);
			g_strncpy(pdir, dir, pos);
			return true;
		}
		else if(pos==0)
		{
			return false;
		}
		else
		{
			pos = auge_find_last_char(dir, '/');
			if(pos>0)
			{
				memset(pdir, 0, size);
				g_strncpy(pdir, dir, pos);
				return true;
			}
			else if(pos==0)
			{
				return false;
			}
		}

		return false;
	}

	g_int auge_mkdir(const char* path)
	{
		if(path==NULL)
		{
			return -1;
		}
#ifdef WIN32
		return mkdir(path);
#else
		return mkdir(path,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
	}

	g_int auge_rmdir(const char* path)
	{
		if(path==NULL)
		{
			return -1;
		}
		if(g_access(path,4))
		{
			return -1;
		}
		return rmdir(path);
	}

	g_int auge_remove_file(const char* path)
	{
		if(path==NULL)
		{
			return -1;
		}
		if(g_access(path,4))
		{
			return -1;
		}
		
		return remove(path);
	}

	g_int auge_rename(const char* src, const char* des)
	{
		if(src==NULL||des==NULL)
		{
			return -1;
		}
		if(g_access(src,4))
		{
			return -1;
		}
		if(!g_access(des,4))
		{
			return -1;
		}

		return rename(src,des);
	}

	g_int auge_move(const char* src, const char* des)
	{
		return auge_rename(src, des);
	}

	g_int auge_find_last_char(const char *str, char ch)
	{
		if(str==NULL)
			return -1;

		g_int pos = strlen(str) - 1;		
		for(char* ptr=(char*)str+pos; pos>=0; pos--,ptr--)
		{
			if((*ptr) == ch)
			{
				break;
			}
		}
		return pos;
	}

	void auge_str_upper(char* str)
	{
		if(str==NULL)
		{
			return;
		}

		for(char* ptr=str; ptr!=NULL; ptr++)
		{
			if(((*ptr)>='a')&&((*ptr)<='z'))
			{
				(*ptr) -= 32;
			}
		}
		
	}

	void auge_str_lower(char* str)
	{
		if(str==NULL)
		{
			return;
		}

		for(char* ptr=str; ptr!=NULL; ptr++)
		{
			if(((*ptr)>='A')&&((*ptr)<='Z'))
			{
				(*ptr) += 32;
			}
		}
	}

	void* auge_load_library(const char* path)
	{
		if(path==NULL)
		{
			return NULL;
		}

		void* handler = NULL;
#ifdef WIN32
		handler = ::LoadLibrary(path);
#else
		handler = dlopen(path, RTLD_NOW);
#endif
		if(handler==NULL)
		{
			GError* pError = augeGetErrorInstance();
#ifdef WIN32
			DWORD dw = ::GetLastError();
			LPTSTR errmsg = NULL;
			::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
							FORMAT_MESSAGE_FROM_SYSTEM,
							NULL,
							dw,
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
							(LPTSTR) &errmsg,
							0, NULL );
			pError->SetError((char*)errmsg);
			::LocalFree(errmsg);
#else
			char* errmsg = dlerror();
			pError->SetError(errmsg);
#endif		
			return NULL;
		}

		return handler;
	}

	void  auge_free_library(void* handler)
	{
		if(handler==NULL)
		{
			return;
		}
#ifdef WIN32
		::FreeLibrary((HINSTANCE)handler);
#else
		dlclose(handler);
#endif
	}

	void* auge_get_function_pointer(void* handler, const char* fname)
	{
		if(handler==NULL||fname==NULL)
		{
			return NULL;
		}
		void* ptr = NULL;
#ifdef WIN32
		ptr = ::GetProcAddress((HINSTANCE)handler,fname);
#else
		ptr = dlsym(handler, fname);
#endif
		if(ptr==NULL)
		{
			GError* pError = augeGetErrorInstance();
#ifdef WIN32
			DWORD dw = ::GetLastError();
			LPTSTR errmsg = NULL;
			::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
							FORMAT_MESSAGE_FROM_SYSTEM,
							NULL,
							dw,
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
							(LPTSTR) &errmsg,
							0, NULL );
			pError->SetError((char*)errmsg);
			::LocalFree(errmsg);
#else
			char* errmsg = dlerror();
			pError->SetError(errmsg);
#endif
		}
		return ptr;
	}

	void auge_generate_uuid(char* uuid, size_t size)
	{
#ifdef WIN32
		uuid_t uu;
		UuidCreate(&uu);
		g_uchar* buffer = NULL;
		UuidToString((uuid_t*)&uu, &buffer);
		int len = strlen((char*)buffer);
		memcpy(uuid, buffer, len);
#else
		uuid_t uid;
	       	uuid_generate(uid);
		uuid_unparse(uid, uuid);
		
#endif
	}

	g_int auge_get_image_suffix(const char* mime_type, char* suffix, size_t size)
	{
		if(mime_type==NULL)
		{
			return -1;
		}
		const char* sep = strstr(mime_type, "/");
		if(sep==NULL)
		{
			return -1;
		}
		strcpy(suffix, sep+1);
		return 0;
	}

	const char* auge_encoding_convert(const char* from, const char* to, const char* instr, size_t instrlen)
	{
		if(instr==NULL)
			return NULL;

		static char bufout[AUGE_BUFFER_MAX];
		static char *sin;
		static char *sout;
		size_t inlen  = 0;
		size_t outlen = 0;
		size_t ret = 0;
			
		iconv_t c_pt;
		if((c_pt = iconv_open(to, from)) == (iconv_t)-1)
		{   
			return NULL;
		}

		iconv(c_pt, NULL, NULL, NULL, NULL);
		memset(bufout, 0, AUGE_BUFFER_MAX);

		inlen  = instrlen;
		outlen = AUGE_BUFFER_MAX;
		sin    = (char *)instr;
		sout   = bufout;

#ifdef WIN32
		ret = iconv(c_pt, (const char**)&sin, (size_t *)&inlen, &sout, (size_t *)&outlen);
#else
		ret = iconv(c_pt, &sin, (size_t *)&inlen, &sout, (size_t *)&outlen);
#endif
		if (ret == -1)
		{
			iconv_close(c_pt);
			return NULL;
		}

		iconv_close(c_pt);

		return bufout;
	}

	size_t auge_encoding_convert_2(const char* from, const char* to, const char* instr, size_t instrlen, char* outstr, size_t* outlen)
	{
		if(instr==NULL||outstr==NULL||outlen==NULL)
			return -1;

		char *sin;
		char *sout;
		size_t inlen  = 0;
		int ret = 0;

		//#ifndef _WIN32_WCE

		iconv_t c_pt;

		if((c_pt = iconv_open(to, from)) == (iconv_t)-1)
		{   
			return -1;
		}

		iconv(c_pt, NULL, NULL, NULL, NULL);

		inlen  = instrlen;
		sin    = (char *)instr;
		sout   = outstr;

#ifdef _WIN32
		ret = iconv(c_pt, (const char**)&sin, (size_t *)&inlen, &sout, (size_t *)outlen);
#else
		ret = iconv(c_pt, &sin, (size_t *)&inlen, &sout, (size_t *)&outlen);
#endif
		if (ret == -1)
		{
			iconv_close(c_pt);
			return 0;
		}

		iconv_close(c_pt);
		//#endif

		return *outlen;
	}


#ifndef WIN32
	void _splitpath( const char *path, char *drive, char *dir, char *fname, char *ext )
	{
		if(path==NULL)
			return;
		register char* p = NULL;
		char* last_slash = NULL;
		char* dot = NULL;
		unsigned len;

		if(drive)
			drive = '\0';

		for(last_slash=NULL, p=(char*)path; *p; p++)
		{
			if(*p=='/')
				last_slash = p+1;
			else if(*p=='.')
				dot = p;	
		}
		if(last_slash)
		{
			if(dir)
			{
				int len = last_slash-path;
				strncpy(dir, path, len);
				*(dir+len)	 = '\0';
			}
			path = last_slash;
		}
		else if(dir)
		{
			*dir = '\0';	
		}

		if(dot && (dot>path))
		{
			if(fname)
			{
				len = g_min(((char*)dot-(char*)path)/sizeof(char), (AUGE_NAME_MAX - 1));
				strncpy(fname, path, len);
				*(fname + len) = '\0';
			}

			if(ext)
			{
				len = g_min(((char*)p - (char*)dot) / sizeof(char), (AUGE_EXT_MAX - 1));
				strncpy(ext, dot, len);
				*(ext+len)='\0';
			}
		}
		else
		{
			if(fname)
			{
				len = g_min(((char*)p - (char*) path) /sizeof(char), (AUGE_NAME_MAX - 1));
				strncpy(fname, path, len);
				*(fname+len)='\0';
			}
			if(ext)
			{
				*ext='\0';
			}
		}
	}

	void _makepath( char *path, const char *drive, const char *dir, const char *fname, const char *ext )
	{
		register const char* p = NULL;

		if((p=dir)&&(*p))
		{
			do
			{
				*path++ = *p++;
			}while(*p);

			if(*(p-1)!='/'&&*(p-1)!='\\')
			{
				*(path++)='/';
			}
		}
		if(p=fname)
		{
			while(*p)
			{
				*path++ = *p++;
			}
		}

		if(p=ext)
		{
			if(*p && *p!='.')
			{
				*path++ = '.';
			}
			while(*path++=*p++)
				;
		}
		else
		{
			*path = '\0';
		}	
	}
#endif

	g_ulong auge_get_time()
	{
#ifdef WIN32
		return (g_ulong)GetTickCount();
#else
		return (g_ulong)time(NULL);
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif
	}

	void auge_get_sys_time_as_string(char* sztime, size_t size)
	{
#ifdef WIN32
		SYSTEMTIME now;
		GetSystemTime(&now);
		sprintf(sztime, "%d%d%d%d%d%d%d",now.wYear,
			now.wMonth,
			now.wDay,
			now.wHour,
			now.wMinute,
			now.wSecond,
			now.wMilliseconds);

#else
		time_t now;
		time(&now);
		struct tm* t_tm;   
		t_tm = localtime(&now);
		t_tm->tm_sec;
		sprintf(sztime, "%d%d%d%d%d%d",t_tm->tm_year,
			t_tm->tm_mon,
			t_tm->tm_mday,
			t_tm->tm_hour,
			t_tm->tm_min,
			t_tm->tm_sec);

#endif
	}

	bool auge_unzip(const char* zip_path, const char* dest_path)
	{
		return true;
	}

	bool auge_zip(const char* src_path, const char* zip_path)
	{
		return true;
	}

	const char*	auge_uint_type_encode(augeUnitType unit)
	{
		switch(unit)
		{
		//case augeInches:
		//	break;
		//case augePoints:
		//	break;
		//case augeFeet:
		//	break;
		//case augeYards:
		//	break;
		//case augeMiles:
		//	break;
		//case augeNauticalMiles:
		//	break;
		//case augeMillimeters:
		//	break;
		//case augeCentimeters:
		//	break;
		case augeMeters:
			return "meters";
			break;
		//case augeKilometers:
		//	break;
		//case augeDecimalDegrees:
		//	break;
		//case augeDecimeters:
		//	break;
		//case augeUnitsLast:
		//	break;
		}
		return "";
	}

	augeUnitType auge_uint_type_decode(const char* unit)
	{
		return augeMeters;
	}

	void auge_rgb_2_hsv(g_byte r, g_byte g, g_byte b, float& h, float& s, float& v)
	{
		int min_v, max_v;
		float delta;

		min_v = g_min( r, g_min(g, b) );
		max_v = g_max( r, g_max(g, b) );
		v = (float)max_v; // v
		delta = (float)(max_v - min_v);

		if( max_v != 0 )
		{
			s = delta / max_v; // s
		}
		else
		{
			// r = g = b = 0 // s = 0, v is undefined
			s = 0;
			h = -1;
			return;
		}

		if( r == max_v )
		{
			h = ( g - b ) / delta; // between yellow & magenta
		}
		else if( g == max_v )
		{
			h = 2 + ( b - r ) / delta; // between cyan & yellow
		}
		else
		{
			h = 4 + ( r - g ) / delta; // between magenta & cyan
		}

		h *= 60; // degrees

		if( h < 0 )
		{
			h += 360;
		}
	}
	
	void auge_hsv_2_rgb(float h, float s, float v, g_byte& r, g_byte& g, g_byte& b)
	{
		int i;
		float f, p, q, t;
		if( s == 0 ) 
		{
			// achromatic (grey)
			r = g = b = v;
			return;
		}
		h /= 60; // sector 0 to 5
		i = floor( h );
		f = h - i; // factorial part of h
		p = v * ( 1 - s );
		q = v * ( 1 - s * f );
		t = v * ( 1 - s * ( 1 - f ) );
		switch( i )
		{
		case 0: 
			r = v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = v;
			b = p;
			break;
		case 2:
			r = p;
			g = v;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = v;
			break;
		case 4:
			r = t;
			g = p;
			b = v;
			break;
		default: // case 5:
			r = v;
			g = p;
			b = q;
			break;
		}
	}

	g_byte auge_get_median(g_byte* values, g_uint size)
	{
		if(values==NULL||size==0)
		{
			return 0;
		}

		//for(int k=0; k<size; k++)
		//{
		//	printf("%d\t", values[k]);
		//}

		g_uint m = size>>1;

		g_byte v, temp;
		g_uint i,j,t;
		for(i=0; i<=m; i++)
		{
			v = values[i], t=i;
			for(j=i+1; j<size; j++)
			{
				if(values[j] < v)
				{
					v = values[j];
					t = j;
				}
			}
			if(t!=i)
			{
				temp	  = values[i];
				values[i] = values[t];
				values[t] = temp;
			}
		}

		return v;
	}
}
