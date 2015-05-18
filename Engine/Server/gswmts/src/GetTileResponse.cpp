#include "GetTileResponse.h"
#include "GetTileRequest.h"
#include "AugeWebCore.h"
#include "AugeService.h"
#include "AugeCore.h"
#include "AugeXML.h"
#include "AugeTile.h"

namespace auge
{
	GetTileResponse::GetTileResponse(GetTileRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_pTile = NULL;
	}

	GetTileResponse::~GetTileResponse()
	{
		if(m_pTile!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pTile);
		}
	}

	RESULTCODE GetTileResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		GLogger* pLogger = augeGetLoggerInstance();

		pWriter->WriteHead(m_pRequest->GetMimeType());

		g_uint nBytes = m_pTile->GetSize();
		g_uchar* buffer	= m_pTile->GetData();
		pWriter->Write(buffer, nBytes);

		pWriter->WriteTail();

		return AG_SUCCESS;
	}

	void GetTileResponse::SetTile(Tile* pTile)
	{
		if(m_pTile!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pTile);
		}
		m_pTile = pTile;
	}

}