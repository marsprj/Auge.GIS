#ifndef __AUGE_WMTS_GET_TILE_RESPONSE_H__
#define __AUGE_WMTS_GET_TILE_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class Tile;
	class GetTileRequest;

	class GetTileResponse : public WebResponse
	{
	public:
		GetTileResponse(GetTileRequest* pRequest);
		virtual ~GetTileResponse();

	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetTile(Tile* pTile);

	private:
		GetTileRequest*	m_pRequest;
		Tile*			m_pTile;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__

