#include "CsTask.h"

namespace auge
{
	CsTask::CsTask():
	m_mid(-1)
	{

	}

	CsTask::~CsTask()
	{

	}

	bool CsTask::Create(g_int tid, const char* name, const char* description, g_int mapID, g_int owner)
	{
		m_id = tid;
		m_mid = mapID;
		m_name = name;
		m_owner = owner;
		m_description = description;

		return true;
	}

	g_int CsTask::GetMapID()
	{
		return m_mid;
	}

	g_int CsTask::GetOwner()
	{
		return m_owner;
	}

	const char*	CsTask::GetName()
	{
		return m_name.c_str();
	}

	const char*	CsTask::GetDescription()
	{
		return m_description.c_str();
	}
}