#include "WorkspacePgs.h"
#include "ConnectionPgs.h"
#include "FeatureCursorPgs.h"
#include "FeatureClassPgs.h"

#include <AugeCore.h>

namespace auge
{
	FeatureClassPgs::FeatureClassPgs():
m_pWorkspace(NULL)
{

}

FeatureClassPgs::~FeatureClassPgs()
{
}

const char*	FeatureClassPgs::GetName()
{
	return m_name.c_str();
}

bool FeatureClassPgs::Create(const char* szName, WorkspacePgs* pWorkspace)
{
	if(szName==NULL||pWorkspace==NULL)
	{
		return false;
	}

	m_name = szName;
	m_pWorkspace = pWorkspace;
	return true;
}

void FeatureClassPgs::Release()
{
	if(!ReleaseRef())
	{
		delete this;
	}
}

FeatureCursor* FeatureClassPgs::Query(augeCursorType type/*=augeStaticCursor*/)
{
	FeatureCursorPgs* pCursor = NULL;
	std::string sql;
	sql = "select st_asbinary(the_geom) as the_geom from ";
	sql.append(m_name);

	PGresult* pgResult = NULL;
	pgResult = m_pWorkspace->m_pgConnection.PgExecute(sql.c_str());
	if(pgResult==NULL)
	{
		return NULL;
	}
	pCursor = new FeatureCursorPgs();
	if(!pCursor->Create(this, pgResult))
	{
		pCursor->Release();
		pCursor = NULL;
	}
	return pCursor;
}
}