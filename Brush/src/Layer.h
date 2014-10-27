#ifndef __AUGE_LAYER_H__
#define __AUGE_LAYER_H__

#include "Base.h"
#include "Envelope.h"

#include <string>

namespace auge
{
	typedef enum LayerType
	{
		augeLayerUnknown = 0,
		augeLayerFeature,
		augeLayerRaster
	}LayerType;

	class Map;

	class AUGE_API Layer : public Base
	{
	public:
		Layer();
		virtual ~Layer();

	public:
		const char*		GetName();
		int				SetName(const char* szName);

		Map*			GetMap();
		void			SetMap(Map* pMap);
		
	public:
		virtual LayerType		GetType();
		virtual Envelope&		GetExtent();

		virtual void			Release();

	private:
		std::string		m_name;
		Map*			m_pMap;
	};

}//namespace

#endif //__AUGE_LAYER_H__
