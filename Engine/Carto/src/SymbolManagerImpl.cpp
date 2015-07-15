#include "SymbolManagerImpl.h"
#include "EnumSymbolImpl.h"

#include "CapitalMarkerImpl.h"
#include "CircleMarkerImpl.h"
#include "PentagonMarkerImpl.h"
#include "StarMarkerImpl.h"
#include "SquareMarkerImpl.h"
#include "TriangleMarkerImpl.h"

#include "RailwaySymbolImpl.h"

namespace auge
{
	#define AUGE_MARKER_SYMBOL_SIMPLE	"Simple"
	#define AUGE_MARKER_SYMBOL_CAPITAL	"Capital"
	#define AUGE_MARKER_SYMBOL_CIRCLE	"Circle"
	#define AUGE_MARKER_SYMBOL_PENTAGON	"Pentagon"
	#define AUGE_MARKER_SYMBOL_STAR		"Star"
	#define AUGE_MARKER_SYMBOL_SQUARE	"Square"
	#define AUGE_MARKER_SYMBOL_TRIANGLE	"Triangle"

	#define AUGE_LINE_SYMBOL_SIMPLE		"Simple"
	#define AUGE_LINE_SYMBOL_RAILWAY	"Railway"

	SymbolManager* augeGetSymbolManagerInstance()
	{
		static SymbolManagerImpl g_symbolManger;
		return &g_symbolManger;
	}

	SymbolManagerImpl::SymbolManagerImpl()
	{
		m_marker_symbols = NULL;
		m_line_symbols   = NULL;
		m_region_symbols = NULL;
	}

	SymbolManagerImpl::~SymbolManagerImpl()
	{
		if(m_marker_symbols!=NULL)
		{
			AUGE_SAFE_RELEASE(m_marker_symbols);
		}
		if(m_line_symbols!=NULL)
		{
			AUGE_SAFE_RELEASE(m_line_symbols);
		}
		if(m_region_symbols!=NULL)
		{
			AUGE_SAFE_RELEASE(m_region_symbols);
		}
	}

	void SymbolManagerImpl::Initialize()
	{
		if(m_path.empty())
		{
			char c_dir[AUGE_PATH_MAX];
			char s_dir[AUGE_PATH_MAX];
			memset(c_dir, 0, AUGE_PATH_MAX);
			memset(s_dir, 0, AUGE_PATH_MAX);
			auge_get_cwd(c_dir, AUGE_PATH_MAX);
#ifdef WIN32
			auge_make_path(s_dir, NULL, c_dir, "symbol", NULL);
#else
			char p_dir[AUGE_PATH_MAX];
			memset(p_dir, 0, AUGE_PATH_MAX);
			auge_get_parent_dir(c_dir, p_dir, AUGE_PATH_MAX);
			auge_make_path(s_dir, NULL, p_dir, "symbol", NULL);
#endif
			auge_mkdir(s_dir);
			m_path = s_dir;
		}
	}

	EnumSymbol*	SymbolManagerImpl::GetMarkerSymbols()
	{
		if(m_marker_symbols == NULL)
		{
			m_marker_symbols = new EnumSymbolImpl();
			m_marker_symbols->Add(new CapitalMarkerImpl());
			m_marker_symbols->Add(new CircleMarkerImpl());
			m_marker_symbols->Add(new PentagonMarkerImpl());
			m_marker_symbols->Add(new StarMarkerImpl());
			m_marker_symbols->Add(new SquareMarkerImpl());
			m_marker_symbols->Add(new TriangleMarkerImpl());
		}
		return m_marker_symbols;
	}

	EnumSymbol*	SymbolManagerImpl::GetLineSymbols()
	{
		if(m_line_symbols == NULL)
		{
			m_line_symbols = new EnumSymbolImpl();
		}
		return m_line_symbols;
	}

	EnumSymbol*	SymbolManagerImpl::GetRegionSymbols()
	{
		if(m_region_symbols == NULL)
		{
			m_region_symbols = new EnumSymbolImpl();

		}
		return m_region_symbols;
	}

	MarkerSymbol* SymbolManagerImpl::CreateMarkerSymbol(augeMarkerType type)
	{
		MarkerSymbol* pSymbol = NULL;
		switch(type)
		{
		case augeMarkerSquare:
			pSymbol = new SquareMarkerImpl();
			break;
		case augeMarkerCircle:
			pSymbol = new CircleMarkerImpl();
			break;
		case augeMarkerTriangle:
			pSymbol = new TriangleMarkerImpl();
			break;
		case augeMarkerStar:
			pSymbol = new StarMarkerImpl();
			break;
		case augeMarkerCross:
			break;
		case augeMarkerX:
			break;
		case augeMarkerPentagon:
			pSymbol = new PentagonMarkerImpl();
			break;
		case augeMarkerCapital:
			pSymbol = new CapitalMarkerImpl();
			break;
		default:
			break;
		}
		return pSymbol;
	}

	MarkerSymbol* SymbolManagerImpl::CreateMarkerSymbol(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}

		MarkerSymbol* pSymbol = NULL;
		if(g_stricmp(name, AUGE_MARKER_SYMBOL_SIMPLE)==0)
		{
			pSymbol = NULL;
		}
		else if(g_stricmp(name, AUGE_MARKER_SYMBOL_CAPITAL)==0)
		{
			pSymbol = new CapitalMarkerImpl();
		}
		else if(g_stricmp(name, AUGE_MARKER_SYMBOL_CIRCLE)==0)
		{
			pSymbol = new CircleMarkerImpl();
		}
		else if(g_stricmp(name, AUGE_MARKER_SYMBOL_PENTAGON)==0)
		{
			pSymbol = new PentagonMarkerImpl();
		}
		else if(g_stricmp(name, AUGE_MARKER_SYMBOL_STAR)==0)
		{
			pSymbol = new StarMarkerImpl();
		}
		else if(g_stricmp(name, AUGE_MARKER_SYMBOL_SQUARE)==0)
		{
			pSymbol = new SquareMarkerImpl();
		}
		else if(g_stricmp(name, AUGE_MARKER_SYMBOL_TRIANGLE)==0)
		{
			pSymbol = new TriangleMarkerImpl();
		}
		else
		{
			pSymbol = NULL;
		}

		return pSymbol;

	}

	LineSymbol*	SymbolManagerImpl::CreateLineSymbol(augeLineType type)
	{
		LineSymbol* pSymbol = NULL;
		switch(type)
		{
		case augeLineSimple:
			//pSymbol = new SimpleLineSymbolImpl();
			break;
		case augeLineRailway:
			pSymbol = new RailwaySymbolImpl();
			break;
		default:
			//pSymbol = new SimpleLineSymbolImpl();
			break;
		}
		return pSymbol;
	}

	LineSymbol* SymbolManagerImpl::CreateLineSymbol(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}

		LineSymbol* pSymbol = NULL;
		if(g_stricmp(name, AUGE_LINE_SYMBOL_SIMPLE)==0)
		{
			pSymbol = NULL;
		}
		else if(g_stricmp(name, AUGE_LINE_SYMBOL_RAILWAY)==0)
		{
			pSymbol = new RailwaySymbolImpl();
		}
		else
		{
			pSymbol = NULL;
		}

		return pSymbol;
	}

	void auge_make_symbol_icon_path(const char* icon_name, char* icon_path, size_t size)
	{
		char c_dir[AUGE_PATH_MAX];
		char s_dir[AUGE_PATH_MAX];
		memset(c_dir, 0, AUGE_PATH_MAX);
		memset(s_dir, 0, AUGE_PATH_MAX);
		auge_get_cwd(c_dir, AUGE_PATH_MAX);
#ifdef WIN32
		auge_make_path(s_dir, NULL, c_dir, "symbol", NULL);
#else
		char p_dir[AUGE_PATH_MAX];
		memset(p_dir, 0, AUGE_PATH_MAX);
		auge_get_parent_dir(c_dir, p_dir, AUGE_PATH_MAX);
		auge_make_path(s_dir, NULL, p_dir, "symbol", NULL);
#endif

		memset(icon_path, 0, size);
		auge_make_path(icon_path, NULL, s_dir, icon_name, NULL);
	}
}