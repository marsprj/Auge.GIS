#ifndef __AUGE_RENDERER_H__
#define __AUGE_RENDERER_H__

#ifdef WIN32
#	ifdef AUGE_RENDERER_EXPORTS
#		define AUGE_RENDERER_API __declspec(dllexport)
#	else
#		define AUGE_RENDERER_API __declspec(dllimport)
#	endif
#else
#	define AUGE_RENDERER_API
#endif

#include "AugeGeometry.h"

namespace auge
{
	class Renderer : public GObject
	{
	protected:
		Renderer(){}
		virtual ~Renderer(){}
	public:
	};

	extern "C"
	{
		AUGE_RENDERER_API Renderer augeGetRendererInstance();
	}
}

#endif //__AUGE_RENDERER_H__
