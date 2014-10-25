#ifndef __AUGE_MAP_H__
#define __AUGE_MAP_H__

#include "Base.h"
#include "Envelope.h"

#include <string>
#include <vector>

namespace auge
{
	class Layer;

	class AUGE_API Map
	{
	public:
		Map();
		virtual ~Map();

	public:
		const char*		GetName();
		int				SetName(const char* szName);

		Envelope&		GetExtent();

		vector<Layer*>&	GetLayers();
			
		uint			GetLayerCount();
		Layer*			GetLayer(uint i);
		Layer*			GetLayer(const char* name);

		uint			AddLayer(Layer* pLayer);
		uint			RemoveLayer(uint i);
		uint			RemoveLayer(const char* name);

	public:

		virtual void	Release();

	private:
		void	Cleanup();

	private:

		std::string		m_name;
		vector<Layer*>	m_layers;
		Envelope		m_extent;
	};

}//namespace

#endif //__AUGE_MAP_H__
