#include "TileWorkspaceMongo.h"
#include "GoogleCRS84QuadTileStore.h"
#include "TileImpl.h"
#include <math.h>

namespace auge
{
	GoogleCRS84QuadTileStore::GoogleCRS84QuadTileStore()
	{
		m_tile_type = augeTileGoogleCRS84Quad;
		m_start_level = 1;
		m_end_level = 18;
		m_tile_format = "png";
		m_full_extent.Set(-180.0f,-90.0f,180.0f,90.0f);
		m_extent = m_full_extent;
		m_pWorkspace = NULL;

		m_gridfs = NULL;
	}

	GoogleCRS84QuadTileStore::~GoogleCRS84QuadTileStore()
	{
		if(m_gridfs)
		{
			mongoc_gridfs_destroy (m_gridfs);
			m_gridfs = NULL;
		}
	}

	GEnvelope& GoogleCRS84QuadTileStore::GetExtent()
	{
		return m_extent;
	}

	GEnvelope& GoogleCRS84QuadTileStore::GetFullExtent()
	{
		return m_full_extent;
	}

	void GoogleCRS84QuadTileStore::GetTopLeftCorner(double &x, double &y)
	{
		x = -180;
		y = -90;
	}

	g_uint GoogleCRS84QuadTileStore::GetRows(g_uint level)
	{
		float l = level > 20 ? 20 : level;
		return pow(2, l-1);
	}
	
	g_uint GoogleCRS84QuadTileStore::GetCols(g_uint level)
	{
		float l = level > 20 ? 20 : level;
		return pow(2, l);
	}

	const char*	GoogleCRS84QuadTileStore::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void GoogleCRS84QuadTileStore::SetName(const char* name)
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
	
	augeTileType GoogleCRS84QuadTileStore::GetTileType()
	{
		return m_tile_type;
	}

	Tile* GoogleCRS84QuadTileStore::GetTile(g_uint level, g_uint row, g_uint col)
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

	//Tile* GoogleCRS84QuadTileStore::GetTile(g_uint level, g_uint row, g_uint col)
	//{
	//	char key[AUGE_NAME_MAX] = {0};
	//	MakeKey(key, AUGE_NAME_MAX, level, row, col);

	//	bson_t m_query;
	//	bson_init (&m_query);
	//	bson_append_utf8 (&m_query, "key", -1, key, -1);

	//	mongoc_cursor_t *m_cursor = NULL;
	//	m_cursor = mongoc_collection_find (m_collection,
	//									MONGOC_QUERY_NONE,
	//									0,
	//									0,
	//									0,
	//									&m_query,
	//									NULL,  /* Fields, NULL for all. */
	//									NULL); /* Read Prefs, NULL for default */

	//	const bson_t *m_doc = NULL;
	//	if(!mongoc_cursor_next(m_cursor, &m_doc))
	//	{
	//		mongoc_cursor_destroy (m_cursor);
	//		bson_destroy (&m_query);
	//		return NULL;
	//	}

	//	size_t length = 0;
	//	const char* str = bson_as_json(m_doc,&length);

	//	mongoc_cursor_destroy (m_cursor);
	//	bson_destroy (&m_query);

	//	return NULL;
	//}

	RESULTCODE GoogleCRS84QuadTileStore::PutTile(g_uint level, g_uint row, g_uint col, const char* path)
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

	RESULTCODE GoogleCRS84QuadTileStore::PutTile(g_uint level, g_uint row, g_uint col, unsigned char* data, size_t size)
	{
		if(data==NULL)
		{
			return AG_FAILURE;
		}
		char key[AUGE_NAME_MAX] = {0};
		MakeKey(key, AUGE_NAME_MAX, level, row, col);

		

		return AG_FAILURE;
	}

	//RESULTCODE GoogleCRS84QuadTileStore::PutTile(g_uint level, g_uint row, g_uint col, unsigned char* data, size_t size)
	//{
	//	if(data==NULL)
	//	{
	//		return AG_FAILURE;
	//	}
	//	char key[AUGE_NAME_MAX] = {0};
	//	//g_snprintf(key, AUGE_NAME_MAX,  "%dx%dx%d",level,row,col);
	//	MakeKey(key, AUGE_NAME_MAX, level, row, col);

	//	bson_t *b_doc = NULL;
	//	bson_oid_t b_oid;
	//	bson_error_t b_error;
	//	RESULTCODE rc = AG_SUCCESS;

	//	b_doc = bson_new();
	//	bson_oid_init(&b_oid, NULL);
	//	BSON_APPEND_OID(b_doc, "_oid", &b_oid);
	//	BSON_APPEND_UTF8(b_doc, "key", key);
	//	BSON_APPEND_BINARY(b_doc, "value", BSON_SUBTYPE_BINARY, data, size);
	//	//#define BSON_APPEND_BINARY(b,key,subtype,val,len) \

	//	if(!mongoc_collection_insert(m_collection, MONGOC_INSERT_NONE, b_doc, NULL, &b_error))
	//	{
	//		GError* pError = augeGetErrorInstance();
	//		pError->SetError(b_error.message);
	//		GLogger* pLogger = augeGetLoggerInstance();
	//		pLogger->Error(b_error.message);

	//		rc = AG_FAILURE;
	//	}

	//	bson_destroy(b_doc);

	//	return rc;
	//}
	/*
	 * ------------------------------------------------------------
	 * augeTileGoogleCrs84Quad
	 *	[Row]: 0 ~ 2^(l-1)
	 *	[Col]: 0 ~ 2^l
	 * ------------------------------------------------------------
	 */
	RESULTCODE GoogleCRS84QuadTileStore::GetKey(char* key, size_t size, g_uint level, g_uint row, g_uint col)
	{
		if(key==NULL)
		{
			return AG_FAILURE;
		}
		g_snprintf(key, size, "%dx%dx%d",level,row,col);
		return AG_SUCCESS;
	}

	RESULTCODE GoogleCRS84QuadTileStore::GetExtent(GEnvelope& extent, g_uint level, g_uint row, g_uint col)
	{
		double resolution = m_full_extent.GetHeight() / pow(2.0f,(float)(level-1));
		double xmin = m_full_extent.m_xmin + resolution * col;
		double ymax = m_full_extent.m_ymax - resolution * row;
		double xmax = xmin + resolution;
		double ymin = ymax - resolution;
		extent.Set(xmin,ymin,xmax,ymax);
		return AG_SUCCESS;
	}

	RESULTCODE GoogleCRS84QuadTileStore::GetTilePath(char* key, size_t size, g_uint level, g_uint row, g_uint col)
	{
		return AG_SUCCESS;
	}

	RESULTCODE GoogleCRS84QuadTileStore::Create(TileWorkspaceMongo* pWorkspace, mongoc_gridfs_t *mgo_gridfs)
	{
		m_pWorkspace = pWorkspace;
		m_gridfs = mgo_gridfs;
		return AG_SUCCESS;
	}

	RESULTCODE GoogleCRS84QuadTileStore::Create(TileWorkspaceMongo* pWorkspace, const char* name, g_uint start_level, g_uint end_level, GEnvelope& extent)
	{
		m_pWorkspace = pWorkspace;
		m_start_level = start_level;
		m_end_level = end_level;
		m_extent = extent;

		SetName(name);

		return AG_SUCCESS;
	}

	inline
	void GoogleCRS84QuadTileStore::MakeKey(char* key, size_t size, g_uint level, g_uint row, g_uint col)
	{
		g_snprintf(key, size,  "%dx%dx%d",level,row,col);
	}
}