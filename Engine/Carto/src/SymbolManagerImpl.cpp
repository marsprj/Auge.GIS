#include "SymbolManagerImpl.h"
#include "EnumSymbolImpl.h"

#include "CapitalMarkerImpl.h"
#include "CircleMarkerImpl.h"
#include "PentagonMarkerImpl.h"
#include "StarMarkerImpl.h"
#include "SquareMarkerImpl.h"
#include "TriangleMarkerImpl.h"

#include "SimpleLineSymbolImpl.h"
#include "RailwaySymbolImpl.h"

#include "SimpleRegionSymbolImpl.h"
#include "GrassLandSymbolImpl.h"
#include "FerryRegionSymbolImpl.h"
#include "ContinentShelfRegionSymbolImpl.h"

#include "GraphicMarkerSymbolImpl.h"

namespace auge
{
	#define AUGE_MARKER_SYMBOL_SIMPLE	"Simple"
	#define AUGE_MARKER_SYMBOL_CAPITAL	"Capital"
	#define AUGE_MARKER_SYMBOL_CIRCLE	"Circle"
	#define AUGE_MARKER_SYMBOL_PENTAGON	"Pentagon"
	#define AUGE_MARKER_SYMBOL_STAR		"Star"
	#define AUGE_MARKER_SYMBOL_SQUARE	"Square"
	#define AUGE_MARKER_SYMBOL_TRIANGLE	"Triangle"

	#define AUGE_LINE_SYMBOL_SIMPLE		"SimpleLine"
	#define AUGE_LINE_SYMBOL_RAILWAY	"Railway"

	#define AUGE_REGION_SYMBOL_SIMPLE	"SimpleRegion"
	#define AUGE_REGION_SYMBOL_GRASS	"GrassLand"
	#define AUGE_REGION_SYMBOL_FERRY	"Ferry"				//°Ú¶ÉÇø
	#define AUGE_REGION_SYMBOL_CONTINENTAL_SHELF "ContinentalShelf"	//´óÂ½¼Ü

	#define AUGE_GRAPHIC_MARKER_BASE_PATH	"graphic/marker"
	
	SymbolManager* augeGetSymbolManagerInstance()
	{
		static SymbolManagerImpl g_symbolManger;		
		if(!g_symbolManger.IsInitialized())
		{
			g_symbolManger.Initialize();
		}
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

	const char* SymbolManagerImpl::GetPath()
	{
		return m_path.c_str();
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

	bool SymbolManagerImpl::IsInitialized()
	{
		return !m_path.empty();
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

			LoadGraphicMarkers();
		}
		return m_marker_symbols;
	}
	
	MarkerSymbol* SymbolManagerImpl::GetMarker(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}
		Symbol* pSymbol = NULL;
		EnumSymbolImpl* pMarkers = (EnumSymbolImpl*)GetMarkerSymbols();

		std::vector<Symbol*>::iterator iter;
		for(iter=pMarkers->m_symbols.begin(); iter!=pMarkers->m_symbols.end(); iter++)
		{
			pSymbol = *iter;
			if(g_stricmp(pSymbol->GetName(), name)==0)
			{
				pSymbol->AddRef();
				return (MarkerSymbol*)pSymbol;
			}
		}

		return NULL;
	}


	EnumSymbol*	SymbolManagerImpl::GetLineSymbols()
	{
		if(m_line_symbols == NULL)
		{
			m_line_symbols = new EnumSymbolImpl();
			m_line_symbols->Add(new SimpleLineSymbolImpl());
			m_line_symbols->Add(new RailwaySymbolImpl());
		}
		return m_line_symbols;
	}

	EnumSymbol*	SymbolManagerImpl::GetRegionSymbols()
	{
		if(m_region_symbols == NULL)
		{
			m_region_symbols = new EnumSymbolImpl();
			m_region_symbols->Add(new SimpleRegionSymbolImpl());
			m_region_symbols->Add(new FerryRegionSymbolImpl());
			m_region_symbols->Add(new ContinentShelfRegionSymbolImpl());
		}
		return m_region_symbols;
	}

	SimpleMarkerSymbol* SymbolManagerImpl::CreateMarkerSymbol(augeMarkerType type)
	{
		SimpleMarkerSymbol* pSymbol = NULL;
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

	SimpleMarkerSymbol* SymbolManagerImpl::CreateMarkerSymbol(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}

		SimpleMarkerSymbol* pSymbol = NULL;
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
			pSymbol = new SimpleLineSymbolImpl();
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
			pSymbol = new SimpleLineSymbolImpl();
		}
		else if(g_stricmp(name, AUGE_LINE_SYMBOL_RAILWAY)==0)
		{
			pSymbol = new RailwaySymbolImpl();
		}
		else
		{
			pSymbol = new SimpleLineSymbolImpl();
		}

		return pSymbol;
	}

	FillSymbol* SymbolManagerImpl::CreateRegionSymbol(augeRegionType type)
	{
		FillSymbol* pSymbol = NULL;
		switch(type)
		{
		case augeRegionSimple:
			pSymbol = new SimpleRegionSymbolImpl();
			break;
		case augeRegionGrass:
			pSymbol = new GrassLandSymbolImpl();
			break;
		case augeRegionFerry:
			pSymbol = new FerryRegionSymbolImpl();
			break;
		case augeRegionContinentShelf:
			pSymbol = new ContinentShelfRegionSymbolImpl();
			break;
		default:
			pSymbol = new SimpleRegionSymbolImpl();
			break;
		}
		return pSymbol;
	}

	FillSymbol* SymbolManagerImpl::CreateRegionSymbol(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}

		FillSymbol* pSymbol = NULL;
		if(g_stricmp(name, AUGE_REGION_SYMBOL_SIMPLE)==0)
		{
			pSymbol = new SimpleRegionSymbolImpl();
		}
		else if(g_stricmp(name, AUGE_REGION_SYMBOL_GRASS)==0)
		{
			pSymbol = new GrassLandSymbolImpl();
		}
		else if(g_stricmp(name, AUGE_REGION_SYMBOL_FERRY)==0)
		{
			pSymbol = new GrassLandSymbolImpl();
		}
		else
		{
			pSymbol = new SimpleRegionSymbolImpl();
		}

		return pSymbol;
	}

	void SymbolManagerImpl::LoadVectorMarkers()
	{

	}

	void SymbolManagerImpl::LoadGraphicMarkers()
	{
		size_t len = 0;
		char graphic_name[AUGE_NAME_MAX];
		char graphic_path[AUGE_PATH_MAX];
		char file_path[AUGE_PATH_MAX];

#ifdef WIN32
		HANDLE hFind = NULL;
		WIN32_FIND_DATAA wfd;

		char filter[AUGE_PATH_MAX];
		char graphic_filter[AUGE_PATH_MAX];
		auge_make_path(graphic_filter,NULL, AUGE_GRAPHIC_MARKER_BASE_PATH,"*","png");
		auge_make_path(filter,NULL,m_path.c_str(), graphic_filter,NULL);
				
		hFind = ::FindFirstFile(filter, &wfd);
		if(hFind == INVALID_HANDLE_VALUE)
		{
			::FindClose(hFind);
		}

		GraphicMarkerSymbolImpl* pMarker = NULL;
		while(::FindNextFile(hFind, &wfd)==TRUE)
		{
			len = strlen(wfd.cFileName);
			if(len>4)
			{
				memset(graphic_name, 0, AUGE_NAME_MAX);
				memset(graphic_path, 0, AUGE_PATH_MAX);
				memset(file_path, 0, AUGE_PATH_MAX);

				strncpy(graphic_name, wfd.cFileName, len-4);
				auge_make_path(graphic_path, NULL, AUGE_GRAPHIC_MARKER_BASE_PATH, wfd.cFileName, NULL);
				auge_make_path(file_path, NULL, m_path.c_str(), graphic_path, NULL);

				pMarker = new GraphicMarkerSymbolImpl();	
				pMarker->SetName(graphic_name);
				pMarker->SetPath(graphic_path);
				pMarker->SetFilePath(file_path);

				m_marker_symbols->Add(pMarker);
			}
		}
		::FindClose(hFind);
#else
		char local_path[AUGE_NAME_MAX];
		memset(local_path, 0, AUGE_NAME_MAX);
		auge_make_path(local_path,NULL,m_path.c_str(), AUGE_GRAPHIC_MARKER_BASE_PATH, NULL);

		DIR *dp = opendir(local_path);
		if(dp!=NULL)
		{
			struct dirent* dirp = NULL;
			while((dirp = readdir(dp))!=NULL)
			{	
				if(dirp->d_type=='\b')
				{
					len = strlen(dirp->d_name); 
					if(len>4)
					{
						if(g_strnicmp(dirp->d_name+len-4, ".png", 4)==0)
						{
							strncpy(graphic_name, dirp->d_name, len-4);
							memset(graphic_path, 0, AUGE_PATH_MAX);
							memset(file_path, 0, AUGE_PATH_MAX);

							auge_make_path(graphic_path, NULL, AUGE_GRAPHIC_MARKER_BASE_PATH, dirp->d_name, NULL);
							auge_make_path(file_path, NULL, m_path.c_str(), graphic_path, NULL);

							pMarker = new GraphicMarkerSymbolImpl();	
							pMarker->SetName(graphic_name);
							pMarker->SetPath(graphic_path);
							pMarker->SetFilePath(file_path);

							m_marker_symbols->Add(pMarker);
						}
					}
				}
			}
			closedir(dp);
		}
#endif

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

	void auge_make_graphic_marker_symbol_base_path(char* graphic_marker_base_path, size_t size)
	{
		char c_dir[AUGE_PATH_MAX];
		char s_dir[AUGE_PATH_MAX];
		memset(c_dir, 0, AUGE_PATH_MAX);
		memset(s_dir, 0, AUGE_PATH_MAX);
		auge_get_cwd(c_dir, AUGE_PATH_MAX);
#ifdef WIN32
		strcpy(s_dir, c_dir);
#else		
		auge_get_parent_dir(c_dir, s_dir, AUGE_PATH_MAX);
#endif

		memset(graphic_marker_base_path, 0, size);
		auge_make_path(graphic_marker_base_path, NULL, s_dir, "symbol/graphic/marker", NULL);
	}
}