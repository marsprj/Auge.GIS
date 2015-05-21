#include "ArchiveProcessorImpl.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <archive.h>
#include <archive_entry.h>

namespace auge
{
	int copy_data(struct archive *ar, struct archive *aw);

	ArchiveProcessorImpl::ArchiveProcessorImpl()
	{

	}

	ArchiveProcessorImpl::~ArchiveProcessorImpl()
	{
		Clear();
	}

	void ArchiveProcessorImpl::AddFile(const char* fpath)
	{
		if(fpath)
		{
			std::string path = fpath;
			m_paths.push_back(path);
		}
	}

	void ArchiveProcessorImpl::SetArchivePath(const char* apath)
	{
		if(apath==NULL)
		{
			m_archive_path.clear();
		}
		else
		{
			m_archive_path = apath;
		}
	}

	const char* ArchiveProcessorImpl::GetArchivePath()
	{
		return m_archive_path.empty() ? NULL : m_archive_path.c_str();
	}

	void ArchiveProcessorImpl::Clear()
	{
		m_paths.clear();
	}

	RESULTCODE ArchiveProcessorImpl::Compress()
	{
		const char* tpath = GetArchivePath();
		if(tpath==NULL)
		{
			return AG_FAILURE;
		}

		struct archive *a;
		struct archive_entry *entry;
		struct stat st;
		char buff[AUGE_BUFFER_MAX];
		int len;
		int fd;

		a = archive_write_new();
		if(a==NULL)
		{
			return AG_FAILURE;
		}
		//archive_write_add_filter_gzip(a);
		archive_write_set_format_zip(a);
		archive_write_set_format_pax_restricted(a);
		archive_write_open_filename(a,tpath);

		const char* fpath = NULL;
		char name[AUGE_NAME_MAX];
		char ext[AUGE_EXT_MAX];
		char fname[AUGE_NAME_MAX];
		std::vector<std::string>::iterator iter;
		for(iter=m_paths.begin(); iter!=m_paths.end();iter++)
		{
			fpath = (*iter).c_str();
			memset(fname,0,AUGE_NAME_MAX);
			memset(name,0,AUGE_NAME_MAX);
			memset(name,0,AUGE_EXT_MAX);
			auge_split_path(fpath, NULL, NULL, name, ext);
			auge_make_path(fname, NULL, NULL, name, ext);
			if(strlen(fname)==0)
			{
				continue;
			}

			stat(fpath,&st);

			entry = archive_entry_new();
			archive_entry_set_pathname(entry,fname);
			archive_entry_set_size(entry,st.st_size);
			archive_entry_set_filetype(entry, AE_IFREG);
			archive_entry_set_perm(entry,0644);
			archive_write_header(a,entry);

			fd = open(fpath,O_RDONLY);
			len = read(fd,buff,sizeof(buff));
			while(len>0)
			{
				archive_write_data(a,buff, len);
				len = read(fd,buff,sizeof(buff));
			}
			close(fd);
			archive_entry_free(entry);
		}

		archive_write_close(a);	
		archive_write_free(a);	

		return AG_SUCCESS;
	}

	RESULTCODE ArchiveProcessorImpl::Decompress()
	{
		const char* tpath = GetArchivePath();
		if(tpath==NULL)
		{
			return AG_FAILURE;
		}
		struct archive *a;
		struct archive *ext;
		struct archive_entry *entry;
		int flags;
		int r;

		/* Select which attributes we want to restore. */
		flags = ARCHIVE_EXTRACT_TIME;
		flags |= ARCHIVE_EXTRACT_PERM;
		flags |= ARCHIVE_EXTRACT_ACL;
		flags |= ARCHIVE_EXTRACT_FFLAGS;

		a = archive_read_new();
		archive_read_support_format_all(a);
		archive_read_support_compression_all(a);

		ext = archive_write_disk_new();
		archive_write_disk_set_options(ext, flags);
		archive_write_disk_set_standard_lookup(ext);
		if((r = archive_read_open_filename(a, tpath, 10240)))
		{
			return AG_SUCCESS;
		}

		while(true)
		{
			r = archive_read_next_header(a, &entry);
			if( r== ARCHIVE_EOF)
			{
				break;
			}
			if( r < ARCHIVE_OK)
			{
				fprintf(stderr, "%s\n", archive_error_string(a));
			}
			if( r< ARCHIVE_WARN)
			{
				break;
			}

			r = archive_write_header(ext, entry);
			if (r < ARCHIVE_OK)
			{
				fprintf(stderr, "%s\n", archive_error_string(ext));
			}
			else if (archive_entry_size(entry) > 0) 
			{
				r = copy_data(a, ext);
			}
			if (r < ARCHIVE_OK)
			{
				fprintf(stderr, "%s\n", archive_error_string(ext));
			}
			if (r < ARCHIVE_WARN)
			{
				break;
			}

			r = archive_write_finish_entry(ext);
			if (r < ARCHIVE_OK)
			{
				fprintf(stderr, "%s\n", archive_error_string(ext));
			}
			if(r < ARCHIVE_WARN)
			{
				break;
			}
		}	

		archive_read_close(a);
		archive_read_free(a);	

		archive_write_close(ext);
		archive_write_free(ext);


		return r==ARCHIVE_OK ? AG_SUCCESS : AG_FAILURE;
	}

	void ArchiveProcessorImpl::Release()
	{
		delete this;
	}

	int copy_data(struct archive *ar, struct archive *aw)
	{
		int r;
		const void *buff;
		size_t size;
		off_t offset;

		while(true)
		{
			r = archive_read_data_block(ar, &buff, &size, &offset);
			if (r == ARCHIVE_EOF)
			{
				return (ARCHIVE_OK);
			}
			if (r < ARCHIVE_OK)
			{
				return (r);
			}
			r = archive_write_data_block(aw, buff, size, offset);
			if (r < ARCHIVE_OK) {
				fprintf(stderr, "%s\n", archive_error_string(aw));
				return (r);
			}
		}
	}

	RESULTCODE ArchiveProcessorImpl::Execute()
	{
		return AG_SUCCESS;
	}
}
