#include "ListRequest.h"
#include "ListResponse.h"
#include "AugeService.h"

namespace auge
{
	ListResponse::ListResponse(ListRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	ListResponse::~ListResponse()
	{
		g_file_t* f = NULL;
		std::vector<g_file_t*>::iterator iter;
		for(iter=m_files.begin(); iter!=m_files.end(); iter++)
		{
			f = *iter;
			free(f);
		}
		m_files.clear();
	}

	RESULTCODE ListResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		const char* fname = NULL;
		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		XDocument *pxDoc = new XDocument(); 
		// FeatureCollection
		pxRoot = pxDoc->CreateRootNode("Files", NULL,NULL);

		g_file_t* f = NULL;
		std::vector<g_file_t*>::iterator iter;
		for(iter=m_files.begin(); iter!=m_files.end(); iter++)
		{
			f = *iter;
			pxNode = pxRoot->AddChild(f->isfolder ? "Folder" : "File", NULL);
#ifdef WIN32
			fname = auge_encoding_convert("GBK","UTF-8",f->fname, strlen(f->fname));
#else
			fname = f->fname;
#endif
			pxNode->SetAttribute("name",fname,NULL);
		}

		int len = 0;
		g_uchar* buffer = NULL;
		pxDoc->WriteToString(&buffer, len, m_pRequest->GetEncoding(),1);

		pWriter->WriteHead(m_pRequest->GetMimeType());
		pWriter->Write(buffer, len);
		pWriter->WriteTail();
		pxDoc->Release();

		return AG_SUCCESS;
	}

	void ListResponse::AddFile(const char* name, bool isfolder)
	{
		g_file_t* f = (g_file_t*)malloc(sizeof(g_file_t));
		strcpy(f->fname,name);
		f->isfolder = isfolder ? 1 : 0;
		m_files.push_back(f);
	}
}