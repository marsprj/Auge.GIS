#ifndef __AUGE_IMS_DESCRIBE_TILESTORE_RESPONSE_H__
#define __AUGE_IMS_DESCRIBE_TILESTORE_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeTile.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class Service;
	class FeatureClass;
	class Workspace;
	class TileStore;
	class DescribeTileStoreRequest;
	class EnumTileStore;

	class DescribeTileStoreResponse : public WebResponse
	{
	public:
		DescribeTileStoreResponse(DescribeTileStoreRequest* pRequest);
		virtual ~DescribeTileStoreResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	public:
		void		SetTileStores(EnumTileStore* pTileStores);
		void		SetTileStore(TileStore* pTileStore);
		void		SetWebContext(WebContext* pWebContext);

		XDocument*	Write(WebWriter* pWriter, TileStore* pTileStore);
		XDocument*	Write(WebWriter* pWriter, EnumTileStore* pTileStores);

	private:
		void		AddTileStoreNode(XElement* pxTileStore, TileStore* pTileStore);
			
	private:
		TileStore* m_pTileStore;
		EnumTileStore* m_pTileStores;
		DescribeTileStoreRequest *m_pRequest;
		WebContext* m_pWebContext;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
