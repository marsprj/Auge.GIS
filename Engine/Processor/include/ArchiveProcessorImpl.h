#ifndef __AUGE_ARCHIVE_IMPL_H__
#define __AUGE_ARCHIVE_IMPL_H__

#include "AugeType.h"
#include "AugeCore.h"
#include "AugeProcessor.h"
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
		virtual void		SetUser(g_uint user);
		virtual void		AddFile(const char* fpath);
		virtual void		SetArchivePath(const char* apath);
		virtual void		Clear();

		virtual RESULTCODE	Compress();
		virtual RESULTCODE	Decompress();

		virtual RESULTCODE	Execute();

		virtual void		Release();

	public:
		const char*			GetArchivePath();

	private:
		std::vector<std::string>	m_paths;
		std::string m_archive_path;

		g_uint		m_user;
	};
}

#endif //__AUGE_ARCHIVE_IMPL_H__
