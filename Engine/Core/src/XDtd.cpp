#include "AugeXML.h"

namespace auge
{
	XDtd::XDtd(xmlDtdPtr pxdtd):
	m_pxDtd(pxdtd)
	{
		m_pxDtd->_private = this;
	}

	XDtd::~XDtd()
	{

	}

	const char* XDtd::GetName() const
	{
		return (char*)m_pxDtd->name;
	}

	const char* XDtd::GetExternalId() const
	{
		return (char*)m_pxDtd->ExternalID;
	}

	const char* XDtd::GetSystemId() const
	{
		return (char*)m_pxDtd->SystemID;
	}
}

