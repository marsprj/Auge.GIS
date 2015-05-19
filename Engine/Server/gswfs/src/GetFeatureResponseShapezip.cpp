#include "GetFeatureResponseShapezip.h"
#include "GetFeatureRequest.h"
#include "AugeWebCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeXML.h"

namespace auge
{
	GetFeatureShapeZipResponse::GetFeatureShapeZipResponse(GetFeatureRequest* pRequest)
	{
		m_pCursor = NULL;
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	GetFeatureShapeZipResponse::~GetFeatureShapeZipResponse()
	{
		if(m_pCursor!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pCursor);
		}
		if(m_pRequest!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRequest);
		}
	}

	void GetFeatureShapeZipResponse::SetWebContenxt(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}

	RESULTCODE GetFeatureShapeZipResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		char uuid[AUGE_NAME_MAX];
		memset(uuid, 0, AUGE_NAME_MAX);
		auge_generate_uuid(uuid, AUGE_NAME_MAX);
		const char* cach_path = m_pWebContext->GetCachePath();

		RESULTCODE rc = AG_FAILURE;
		rc = Export(cach_path, uuid);

		char zipfile[AUGE_PATH_MAX];
		memset(zipfile, 0, AUGE_PATH_MAX);
		auge_make_path(zipfile, NULL, cach_path, uuid, "shp");

		return WriteFile(pWriter, zipfile);
	}

	void GetFeatureShapeZipResponse::SetFeatureCursor(FeatureCursor* pCursor)
	{
		m_pCursor = pCursor;
	}

	RESULTCODE GetFeatureShapeZipResponse::Write()
	{	
		return AG_SUCCESS;
	}

	RESULTCODE GetFeatureShapeZipResponse::Export(const char* path, const char* className)
	{	
		FeatureClass* psrcFeatureClass = NULL;
		psrcFeatureClass = m_pCursor->GetFeatureClass();

		DataEngine* pshpEngine = NULL;
		DataEngineManager* pDataEngineManager = augeGetDataEngineManagerInstance();
		pshpEngine = pDataEngineManager->GetEngine("Shape");
		if(pshpEngine==NULL)
		{
			return AG_FAILURE;
		}

		char connstr[AUGE_NAME_MAX];
		g_sprintf(connstr, "DATABASE=%s",path);

		RESULTCODE rc = AG_FAILURE;
		FeatureWorksapce* pshpWorkspace = dynamic_cast<FeatureWorksapce*>(pshpEngine->CreateWorkspace());
		pshpWorkspace->SetConnectionString(connstr);
		rc = pshpWorkspace->Open();
		if(rc!=AG_SUCCESS)
		{
			pshpWorkspace->Release();
			return AG_FAILURE;
		}

		FeatureClass* pshpFeatureClass = NULL;
		pshpFeatureClass = pshpWorkspace->CreateFeatureClass(className, psrcFeatureClass->GetFields());
		if(pshpFeatureClass==NULL)
		{
			pshpWorkspace->Release();
			return AG_FAILURE;
		}
		
		FeatureInsertCommand* cmd = pshpFeatureClass->CreateInsertCommand();

		Feature* pFeature = NULL;
		while((pFeature=m_pCursor->NextFeature())!=NULL)
		{
			cmd->Insert(pFeature);
			pFeature->Release();
		}
		cmd->Release();
		pshpFeatureClass->Release();
		pshpWorkspace->Release();

		return AG_FAILURE;
	}

	RESULTCODE GetFeatureShapeZipResponse::WriteFile(WebWriter* pWriter, const char* fpath)
	{
		FILE* fp = fopen(fpath, "rb");
		if(fp==NULL)
		{
			return AG_FAILURE;
		}

		pWriter->WriteHead("application/zip");

		g_uint nBytes = 0;
		g_uchar buffer[AUGE_BUFFER_MAX];		
		memset(buffer, 0, AUGE_BUFFER_MAX);
		while((nBytes=fread(buffer, sizeof(g_uchar),AUGE_BUFFER_MAX, fp))>0)
		{
			pWriter->Write(buffer, nBytes);
		}

		pWriter->WriteTail();

		fclose(fp);

		return AG_SUCCESS;
	}
}