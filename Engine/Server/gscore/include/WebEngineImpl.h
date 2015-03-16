#ifndef __AUGE_WEB_ENGINE_IMPL_H__
#define __AUGE_WEB_ENGINE_IMPL_H__

#include "AugeService.h"

namespace auge
{
	class WebEngineImpl : public WebEngine
	{
	public:
		WebEngineImpl();
		virtual ~WebEngineImpl();
	public:
		virtual const char*		GetName();
	};
}

#endif //__AUGE_WEB_ENGINE_IMPL_H__
