#ifndef __AUGE_ARCHIVE_IMPL_H__
#define __AUGE_ARCHIVE_IMPL_H__

#include "AugeType.h"
#include "AugeCore.h"
#include <vector>
#include <string>

namespace auge
{
	class ArchiveProcessorImpl : public ArchiveProcessor
	{
	public:
		ArchiveProcessorImpl();
		virtual ~ArchiveProcessorImpl();
	public:
		virtual void		AddFile(const char* fpath);
		virtual void		Clear();
		virtual RESULTCODE	Compress(const char* tpath);
		virtual RESULTCODE	Decompress(const char* tpath);
		virtual void		Release();
	private:
		std::vector<std::string>	m_paths;
	};
}

#endif //__AUGE_ARCHIVE_IMPL_H__
