#include "GetMapResponse.h"
#include "GetMapRequest.h"
#include "AugeWebCore.h"

namespace auge
{
	GetMapResponse::GetMapResponse(GetMapRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}
	
	GetMapResponse::~GetMapResponse()
	{

	}

	RESULTCODE GetMapResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		GLogger* pLogger = augeGetLoggerInstance();
		pLogger->Debug(m_path.c_str(), __FILE__, __LINE__);
		
		FILE* fp = fopen(m_path.c_str(), "rb");
		if(fp==NULL)
		{
			return AG_FAILURE;
		}

		pWriter->WriteHead(m_pRequest->GetMimeType());

		g_ulong ts, te;
		char msg[AUGE_MSG_MAX];
		ts = auge_get_time();

		g_uint nBytes = 0;
		g_uchar buffer[AUGE_BUFFER_MAX];		
		memset(buffer, 0, AUGE_BUFFER_MAX);
		while((nBytes=fread(buffer, sizeof(g_uchar),AUGE_BUFFER_MAX, fp))>0)
		{
			pWriter->Write(buffer, nBytes);
		}

		te = auge_get_time();
		g_sprintf(msg, "[WMS Image Write]:%ld ms", te-ts);
		pLogger->Debug(msg, __FILE__, __LINE__);
		
		pWriter->WriteTail();

		fclose(fp);

		MoveToThumbnail(m_path.c_str());

		return AG_SUCCESS;
	}

	void GetMapResponse::SetPath(const char* path)
	{
		m_path = path;
	}

	void GetMapResponse::MoveToThumbnail(const char* img_path)
	{
		char img_name[AUGE_NAME_MAX];
		char img_ext[AUGE_EXT_MAX];
		memset(img_name, 0, AUGE_NAME_MAX);
		memset(img_ext, 0, AUGE_EXT_MAX);
		auge_split_path(img_path, NULL, NULL, img_name, img_ext);

		WebContext* pWebContext = augeGetWebContextInstance();
		char thumbnail_path[AUGE_PATH_MAX];
		memset(thumbnail_path, 0, AUGE_PATH_MAX);
		auge_make_path(thumbnail_path, NULL, pWebContext->GetThumbnailPath(), img_name, img_ext);
		
		auge_move(img_path, thumbnail_path);
	}
}