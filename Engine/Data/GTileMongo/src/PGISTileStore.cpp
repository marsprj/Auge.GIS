#include "TileWorkspaceMongo.h"
#include "PGISTileStore.h"
#include "TileImpl.h"
#include <math.h>

namespace auge
{
	PGISTileStore::PGISTileStore()
	{
		m_tile_type = augeTileGoogleCRS84Quad;
		m_start_level = 1;
		m_end_level = 18;
		m_tile_format = "png";
		m_full_extent.Set(-256.0f,-256.0f,256.0f,256.0f);
		m_extent.Set(-180.0f,-90.0f,180.0f,90.0f);

		m_pWorkspace = NULL;
	}

	PGISTileStore::~PGISTileStore()
	{

	}

	GEnvelope& PGISTileStore::GetExtent()
	{
		return m_extent;
	}

	GEnvelope& PGISTileStore::GetFullExtent()
	{
		return m_full_extent;
	}

	void PGISTileStore::GetTopLeftCorner(double &x, double &y)
	{
		x = -256;
		y = -256;
	}

	g_uint64 PGISTileStore::GetRows(g_uint level)
	{
		float l = level > 20 ? 20 : level;
		return pow(2, l);
	}
	
	g_uint64 PGISTileStore::GetCols(g_uint level)
	{
		float l = level > 20 ? 20 : level;
		return pow(2, l);
	}
	
	g_uint64 PGISTileStore::GetBlockRows(g_uint level)
	{
		return GetRows(level) / (AUGE_TILE_BLOCK_SIZE >> 8);
	}

	g_uint64 PGISTileStore::GetBlockCols(g_uint level)
	{
		return GetCols(level) / (AUGE_TILE_BLOCK_SIZE >> 8);
	}

	TileBlock* PGISTileStore::GetBlock(g_uint level, g_uint64 row, g_uint64 col)
	{
		return NULL;
	}

	const char*	PGISTileStore::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void PGISTileStore::SetName(const char* name)
	{
		if(name)
		{
			m_name = name;
		}
		else
		{
			m_name.clear();
		}
	}
	
	augeTileType PGISTileStore::GetTileType()
	{
		return m_tile_type;
	}

	const char* PGISTileStore::GetTileTypeAsString()
	{
		return AUGE_TILE_TYPE_PGIS;
	}

	Tile* PGISTileStore::GetTile(g_uint level, g_uint64 row, g_uint64 col)
	{
		char key[AUGE_NAME_MAX] = {0};
		MakeKey(key, AUGE_NAME_MAX, level, row, col);

		bson_error_t error;
		mongoc_gridfs_file_t *mgo_file = NULL;
		mgo_file = mongoc_gridfs_find_one_by_filename(m_gridfs, key, &error);
		if(!mgo_file)
		{
			return NULL;
		}

		mongoc_stream_t *mgo_stream = NULL;
		mgo_stream = mongoc_stream_gridfs_new (mgo_file);
		if(!mgo_stream)
		{
			mongoc_gridfs_file_destroy(mgo_file);
			return NULL;
		}

		int64_t length = mongoc_gridfs_file_get_length(mgo_file);
		g_uchar* buffer = (g_uchar*)malloc(length*sizeof(g_uchar));
		memset(buffer, 0, length);

		g_uchar* ptr = buffer;
		mongoc_iovec_t iov;
		ssize_t r;

		iov.iov_base = (char*)buffer;
		iov.iov_len = length;

		r = mongoc_stream_readv (mgo_stream, &iov, 1, -1, 0);
		if(r<=0)
		{
			mongoc_stream_destroy (mgo_stream);
			mongoc_gridfs_file_destroy(mgo_file);
			return NULL;
		}

		mongoc_stream_destroy (mgo_stream);
		mongoc_gridfs_file_destroy(mgo_file);

		TileImpl* pTile = new TileImpl();
		pTile->SetData(buffer);
		pTile->SetSize(length);
		pTile->SetKey(key);

		return pTile;
	}

	RESULTCODE PGISTileStore::PutTile(g_uint level, g_uint64 row, g_uint64 col, const char* path)
	{
		if(path==NULL)
		{
			return AG_FAILURE;
		}

		if(g_access(path,4))
		{
			return AG_FAILURE;
		}

		char key[AUGE_NAME_MAX] = {0};
		MakeKey(key, AUGE_NAME_MAX, level, row, col);

		mongoc_stream_t *mgo_stream = NULL;
		mgo_stream = mongoc_stream_file_new_for_path(path, O_RDONLY, 0);
		if(mgo_stream==NULL)
		{
			return AG_FAILURE;
		}

		mongoc_gridfs_file_opt_t opt = { 0 };
		mongoc_gridfs_file_t *mgo_file = NULL;

		opt.filename = key;
		mgo_file = mongoc_gridfs_create_file_from_stream(m_gridfs, mgo_stream, &opt);
		if(mgo_file==NULL)
		{
			return AG_FAILURE;
		}

		bool ret = mongoc_gridfs_file_save(mgo_file);
		mongoc_gridfs_file_destroy(mgo_file);

		return ret ? AG_SUCCESS : AG_FAILURE;
	}

	RESULTCODE PGISTileStore::PutTile(g_uint level, g_uint64 row, g_uint64 col, unsigned char* data, size_t size)
	{
		return AG_SUCCESS;
	}
	/*
	 * ------------------------------------------------------------
	 * augeTileGoogleCrs84Quad
	 *	[Row]: 0 ~ 2^(l-1)
	 *	[Col]: 0 ~ 2^l
	 * ------------------------------------------------------------
	 */
	RESULTCODE PGISTileStore::GetKey(char* key, size_t size, g_uint level, g_uint64 row, g_uint64 col)
	{
		if(key==NULL)
		{
			return AG_FAILURE;
		}
		g_snprintf(key, size, "%dx%dx%d",level,row,col);
		return AG_SUCCESS;
	}

	RESULTCODE PGISTileStore::GetTileExtent(GEnvelope& extent, g_uint level, g_uint64 row, g_uint64 col)
	{
		double resolution = m_full_extent.GetHeight() / pow(2.0f,(float)(level));
		double xmin = m_full_extent.m_xmin + resolution * col;
		double ymax = m_full_extent.m_ymax - resolution * row;
		double xmax = xmin + resolution;
		double ymin = ymax - resolution;
		extent.Set(xmin,ymin,xmax,ymax);

		if(!m_extent.Intersects(extent))
		{
			return AG_FAILURE;
		}
		return AG_SUCCESS;
	}

	RESULTCODE PGISTileStore::GetTilePath(char* key, size_t size, g_uint level, g_uint64 row, g_uint64 col)
	{	
		char l_tile[AUGE_NAME_MAX] = {0};
		char r_tile[AUGE_NAME_MAX] = {0};
		char c_tile[AUGE_NAME_MAX] = {0};
		g_sprintf(l_tile,"L%02d", level);
		g_sprintf(r_tile,"R%08x", row);
		g_sprintf(c_tile,"C%08x", col);

		char l_path[AUGE_PATH_MAX] = {0};
		auge_make_path(l_path, NULL, m_layers_path.c_str(), l_tile, NULL);

		char r_path[AUGE_PATH_MAX] = {0};
		auge_make_path(r_path, NULL, l_path, r_tile, NULL);
		auge_mkdir(r_path);

		//char t_path[AUGE_PATH_MAX] = {0};
		auge_make_path(key, NULL, r_path, c_tile, m_tile_format.c_str());

		//g_snprintf(key, size, "%s/%d/%d_%d.%s",m_path.c_str(), level, row, col,m_tile_format.c_str());
		return AG_SUCCESS;
	}

	RESULTCODE PGISTileStore::Create(TileWorkspaceMongo* pWorkspace, mongoc_gridfs_t *mgo_gridfs, const char* name)
	{
		m_pWorkspace = pWorkspace;
		m_gridfs = mgo_gridfs;

		m_name = name;

		return AG_SUCCESS;
	}

	RESULTCODE PGISTileStore::Create(TileWorkspaceMongo* pWorkspace, const char* name, g_uint start_level, g_uint end_level, GEnvelope& extent)
	{
		m_pWorkspace = pWorkspace;
		m_start_level = start_level;
		m_end_level = end_level;
		m_extent = extent;
		SetName(name);
		return AG_SUCCESS;
	}
	
	RESULTCODE PGISTileStore::GetBoundingBox(GEnvelope& viewer, g_uint level, g_uint& r_min, g_uint& r_max, g_uint& c_min, g_uint& c_max)
	{
		return AG_SUCCESS;
	}

	g_uint PGISTileStore::GetOriginalLevel(GEnvelope& viewer, g_uint viewer_w, g_uint viewer_h)
	{
		return 0;
	}

	inline
	void PGISTileStore::MakeKey(char* key, size_t size, g_uint level, g_uint64 row, g_uint64 col)
	{
		g_snprintf(key, size,  "%dx%lldx%lld",level,row,col);
	}
}