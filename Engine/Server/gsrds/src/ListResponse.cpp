#include "ListRequest.h"
#include "ListResponse.h"
#include "AugeService.h"
#include "AugeData.h"

#include <string.h>
#include <math.h>
#include <time.h>

namespace auge
{
	char* raster_ext[]= {".jpg",".jpeg",".png",".tif",".tiff"}; 
	ListResponse::ListResponse(ListRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();

		m_pFolder = NULL;
	}

	ListResponse::~ListResponse()
	{
		if(m_pFolder!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pFolder);
		}
	}

	void ListResponse::SetFolder(RasterFolder* pFolder)
	{
		m_pFolder = pFolder;
	}

	RESULTCODE ListResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		XDocument *pxDoc = WriteDocument();


//		g_file_t* f = NULL;
//		std::vector<g_file_t*>::iterator iter;
//		for(iter=m_files.begin(); iter!=m_files.end(); iter++)
//		{
//			f = *iter;
//
//			if((!f->isfolder) && (!IsRaster(f->fname)))
//			{
//				continue;
//			}
//
//			pxNode = pxRoot->AddChild(f->isfolder ? "Folder" : "File", NULL);
//#ifdef WIN32
//			fname = auge_encoding_convert("GBK","UTF-8",f->fname, strlen(f->fname));
//#else
//			fname = f->fname;
//#endif
//			// name 
//			pxNode->SetAttribute("name",fname,NULL);
//			// last access time
//			strftime(str,AUGE_NAME_MAX,"%Y-%m-%d %H:%M:%S", localtime((const time_t *)&(f->fstat.st_atime)));
//			pxNode->SetAttribute("access_time",str,NULL);
//			//pxNode->SetAttribute("access_time",ctime((const time_t *)&(f->fstat.st_atime)),NULL);
//			// last modified time
//			strftime(str,AUGE_NAME_MAX,"%Y-%m-%d %H:%M:%S", localtime((const time_t *)&(f->fstat.st_mtime)));
//			pxNode->SetAttribute("last_modified_time",str,NULL);
//			//pxNode->SetAttribute("last_modified_time",ctime((const time_t *)&(f->fstat.st_mtime)),NULL);
//
//			if(!f->isfolder) 
//			{ 
//				// file size
//				memset(str, 0, AUGE_NAME_MAX);
//				g_sprintf(str, "%d KB", (int)((f->fstat.st_size>>10))+1);
//				pxNode->SetAttribute("size",str,NULL);
//			}
//		}

		int len = 0;
		g_uchar* buffer = NULL;
		pxDoc->WriteToString(&buffer, len, m_pRequest->GetEncoding(),1);

		pWriter->WriteHead(m_pRequest->GetMimeType());
		pWriter->Write(buffer, len);
		pWriter->WriteTail();
		pxDoc->Release();

		return AG_SUCCESS;
	}

	
	XDocument* ListResponse::WriteDocument()
	{
		EnumRaster* pRasters = NULL;
		EnumRasterFolder* pFolders = NULL;
		RasterDataset* pRasterDataset = NULL;

		pFolders = m_pFolder->GetFolders();
		//pRasters = m_pFolder->GetRasterDataset()->GetRasters();
		pRasterDataset = m_pFolder->GetRasterDataset();
		if(pRasterDataset!=NULL)
		{
			pRasters  = pRasterDataset->GetRasters();
		}
		
		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		XDocument *pxDoc = new XDocument(); 
		// FeatureCollection
		pxRoot = pxDoc->CreateRootNode("Files", NULL,NULL);

		WriteFolders(pxRoot, pFolders);
		WriteRasters(pxRoot, pRasters);

		pRasters->Release();
		pFolders->Release();
		
		return pxDoc;
	}
	
	void ListResponse::WriteFolders(XElement* pxFiles, EnumRasterFolder* pFolders)
	{
		char str[AUGE_NAME_MAX];

		const char* fname = NULL;
		XElement* pxNode = NULL;

		RasterFolder* pFolder = NULL;
		pFolders->Reset();
		while((pFolder=pFolders->Next())!=NULL)
		{
			pxNode = pxFiles->AddChild("Folder", NULL);			
//#ifdef WIN32
//			fname = auge_encoding_convert(AUGE_ENCODING_GBK,AUGE_ENCODING_UTF8,pFolder->GetName(), strlen(pFolder->GetName()));
//#else
//			fname = pFolder->GetName();
//#endif
			fname = auge_encoding_convert(AUGE_ENCODING_GBK,AUGE_ENCODING_UTF8,pFolder->GetName(), strlen(pFolder->GetName()));
			pxNode->SetAttribute("name",fname,NULL);

			//falias = auge_encoding_convert(AUGE_ENCODING_GBK,AUGE_ENCODING_UTF8,pFolder->getalias(), strlen(pFolder->GetName()));
			pxNode->SetAttribute("alias",fname,NULL);

			const char* fpath = pFolder->GetLocalPath();
			if(!g_access(fpath,4))
			{
#ifdef WIN32
				struct _stat fstat;
				_stat(fpath, &fstat);
#else
				struct stat fstat;
				stat(fpath, &fstat);
#endif
				//last access time
				strftime(str,AUGE_NAME_MAX,"%Y-%m-%d %H:%M:%S", localtime((const time_t *)&(fstat.st_atime)));
				pxNode->SetAttribute("access_time",str,NULL);
				//pxNode->SetAttribute("access_time",ctime((const time_t *)&(f->fstat.st_atime)),NULL);
				// last modified time
				strftime(str,AUGE_NAME_MAX,"%Y-%m-%d %H:%M:%S", localtime((const time_t *)&(fstat.st_mtime)));
				pxNode->SetAttribute("last_modified_time",str,NULL);
				//pxNode->SetAttribute("last_modified_time",ctime((const time_t *)&(f->fstat.st_mtime)),NULL);
			}
		}
	}

	void ListResponse::WriteRasters(XElement* pxFiles, EnumRaster* pRasters)
	{
		if(pRasters==NULL)
		{
			return;
		}
		char str[AUGE_NAME_MAX];

		const char* fname = NULL;
		XElement* pxNode = NULL;

		Raster* pRaster = NULL;
		pRasters->Reset();
		while((pRaster=pRasters->Next())!=NULL)
		{
			pxNode = pxFiles->AddChild("File", NULL);
#ifdef WIN32
			fname = auge_encoding_convert("GBK","UTF-8",pRaster->GetName(), strlen(pRaster->GetName()));
#else
			fname = pRaster->GetName();
#endif
			pxNode->SetAttribute("name",fname,NULL);

			const char* fpath = pRaster->GetPath();
#ifdef WIN32
			struct _stat fstat;
			_stat(fpath, &fstat);
#else
			struct stat fstat;
			stat(fpath, &fstat);
#endif
			//last access time
			strftime(str,AUGE_NAME_MAX,"%Y-%m-%d %H:%M:%S", localtime((const time_t *)&(fstat.st_atime)));
			pxNode->SetAttribute("access_time",str,NULL);
			//pxNode->SetAttribute("access_time",ctime((const time_t *)&(f->fstat.st_atime)),NULL);
			// last modified time
			strftime(str,AUGE_NAME_MAX,"%Y-%m-%d %H:%M:%S", localtime((const time_t *)&(fstat.st_mtime)));
			pxNode->SetAttribute("last_modified_time",str,NULL);
			//pxNode->SetAttribute("last_modified_time",ctime((const time_t *)&(f->fstat.st_mtime)),NULL);
			
			// file size
			memset(str, 0, AUGE_NAME_MAX);
			g_sprintf(str, "%d KB", (int)((fstat.st_size>>10))+1);
			pxNode->SetAttribute("size",str,NULL);
		}
	}

//	RESULTCODE ListResponse::Write(WebWriter* pWriter)
//	{
//		if(pWriter==NULL)
//		{
//			return AG_FAILURE;
//		}
//
//		char str[AUGE_NAME_MAX];
//		const char* fname = NULL;
//		XElement  *pxNode = NULL;
//		XElement  *pxRoot = NULL;
//		XDocument *pxDoc = new XDocument(); 
//		// FeatureCollection
//		pxRoot = pxDoc->CreateRootNode("Files", NULL,NULL);
//		
//		g_file_t* f = NULL;
//		std::vector<g_file_t*>::iterator iter;
//		for(iter=m_files.begin(); iter!=m_files.end(); iter++)
//		{
//			f = *iter;
//
//			if((!f->isfolder) && (!IsRaster(f->fname)))
//			{
//				continue;
//			}
//			 
//			pxNode = pxRoot->AddChild(f->isfolder ? "Folder" : "File", NULL);
//#ifdef WIN32
//			fname = auge_encoding_convert("GBK","UTF-8",f->fname, strlen(f->fname));
//#else
//			fname = f->fname;
//#endif
//			// name 
//			pxNode->SetAttribute("name",fname,NULL);
//			// last access time
//			strftime(str,AUGE_NAME_MAX,"%Y-%m-%d %H:%M:%S", localtime((const time_t *)&(f->fstat.st_atime)));
//			pxNode->SetAttribute("access_time",str,NULL);
//			//pxNode->SetAttribute("access_time",ctime((const time_t *)&(f->fstat.st_atime)),NULL);
//			// last modified time
//			strftime(str,AUGE_NAME_MAX,"%Y-%m-%d %H:%M:%S", localtime((const time_t *)&(f->fstat.st_mtime)));
//			pxNode->SetAttribute("last_modified_time",str,NULL);
//			//pxNode->SetAttribute("last_modified_time",ctime((const time_t *)&(f->fstat.st_mtime)),NULL);
//
//			if(!f->isfolder) 
//			{ 
//				// file size
//				memset(str, 0, AUGE_NAME_MAX);
//				g_sprintf(str, "%d KB", (int)((f->fstat.st_size>>10))+1);
//				pxNode->SetAttribute("size",str,NULL);
//			}
//		}
//
//		int len = 0;
//		g_uchar* buffer = NULL;
//		pxDoc->WriteToString(&buffer, len, m_pRequest->GetEncoding(),1);
//
//		pWriter->WriteHead(m_pRequest->GetMimeType());
//		pWriter->Write(buffer, len);
//		pWriter->WriteTail();
//		pxDoc->Release();
//
//		return AG_SUCCESS;
//	}
//
//	void ListResponse::AddFile(const char* name)
//	{
//		char fpath[AUGE_PATH_MAX];
//		memset(fpath,0,AUGE_PATH_MAX);
//		auge_make_path(fpath, NULL, m_path.c_str(), name,NULL);
//
//		g_file_t* f = (g_file_t*)malloc(sizeof(g_file_t));
//		strcpy(f->fname,name);	
//
//#ifdef WIN32
//		_stat(fpath, &(f->fstat));
//#else
//		stat(fpath, &(f->fstat));
//#endif
//		f->isfolder = S_ISDIR(f->fstat.st_mode) ? true : false;
//		 
//		m_files.push_back(f);
//	}
//
//	bool ListResponse::IsRaster(const char* fname)
//	{
//		char ext[AUGE_EXT_MAX];
//		memset(ext, 0, AUGE_EXT_MAX);
//		auge_split_path(fname, NULL, NULL,NULL,ext);
//
//		size_t count = sizeof(raster_ext) / sizeof(char*);
//		for(size_t i=0; i<count; i++)
//		{
//			if(g_stricmp(raster_ext[i], ext)==0)
//			{
//				return true;
//			}
//		}
//
//		return false;
//	}
}
