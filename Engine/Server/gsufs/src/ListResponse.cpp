#include "ListRequest.h"
#include "ListResponse.h"
#include "AugeService.h"

#include <math.h>
#include <time.h>

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

	void ListResponse::SetPath(const char* path)
	{
		m_path = path;
	}

	RESULTCODE ListResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		char str[AUGE_NAME_MAX];
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
			// name 
			pxNode->SetAttribute("name",fname,NULL);
			// last access time
			strftime(str,AUGE_NAME_MAX,"%Y-%m-%d %H:%M:%S", localtime((const time_t *)&(f->fstat.st_atime)));
			pxNode->SetAttribute("access_time",str,NULL);
			//pxNode->SetAttribute("access_time",ctime((const time_t *)&(f->fstat.st_atime)),NULL);
			// last modified time
			strftime(str,AUGE_NAME_MAX,"%Y-%m-%d %H:%M:%S", localtime((const time_t *)&(f->fstat.st_mtime)));
			pxNode->SetAttribute("last_modified_time",str,NULL);
			//pxNode->SetAttribute("last_modified_time",ctime((const time_t *)&(f->fstat.st_mtime)),NULL);

			if(!f->isfolder) 
			{ 
				AddFileSizeAttribute(pxNode, (int)((f->fstat.st_size>>10))+1);
				//// file size
				//memset(str, 0, AUGE_NAME_MAX);
				//g_sprintf(str, "%d KB", (int)((f->fstat.st_size>>10))+1);
				//pxNode->SetAttribute("size",str,NULL);
			}
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

	void ListResponse::AddFile(const char* name)
	{
		char fpath[AUGE_PATH_MAX];
		memset(fpath,0,AUGE_PATH_MAX);
		auge_make_path(fpath, NULL, m_path.c_str(), name,NULL);

		g_file_t* f = (g_file_t*)malloc(sizeof(g_file_t));
		strcpy(f->fname,name);	

#ifdef WIN32
		_stat(fpath, &(f->fstat));
#else
		stat(fpath, &(f->fstat));
#endif
		f->isfolder = S_ISDIR(f->fstat.st_mode) ? true : false;

		m_files.push_back(f);
	}

	void ListResponse::AddFileSizeAttribute(XElement* pxNode, g_uint64 size)
	{
		static char* g_s_size_quota[] = {"KB", "MB","GB","TB","PB"};
		char str[AUGE_NAME_MAX];
		memset(str, 0, AUGE_NAME_MAX);

		int i=0;
		float fsize=size;
		g_uint64 temp = size>>10;
		for(i=0; temp>0; i++)
		{
			temp = temp >> 10;
			fsize /= 1024;
		}

		g_sprintf(str, "%3.2f %s", fsize, g_s_size_quota[i]);
		pxNode->SetAttribute("size",str,NULL);
	} 
}