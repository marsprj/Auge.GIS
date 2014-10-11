#include "ngx_http_auge_stream.h"

static ngx_int_t 
auge_upstream_create_request(ngx_http_request_t *r)
{
	static ngx_str_t backendQueryLine = ngx_string("GET /geoserver/radi/ows?service=wfs&request=getcapabilities&version=1.0.0 HTTP/1.1 \r\nHost:http://192.168.111.157:8080\r\nConnection:close\r\n\r\n");
	ngx_int_t queryLineLen = backendQueryLine.len - 1;

	ngx_buf_t* b = ngx_create_temp_buf(r->pool, queryLineLen);
	if(b==NULL)
	{
		return NGX_ERROR;
	}

	b->last = b->pos + queryLineLen;

	r->upstream->request_bufs = ngx_alloc_chain_link(r->pool);
	if(r->upstream->request_bufs == NULL)
	{
		return NGX_ERROR;
	}

	r->upstream->request_bufs->buf = b;
	r->upstream->request_bufs->next = NULL;

	r->upstream->request_send = 0;
	r->upstream->header_send = 0;

	r->header_hash = 1;
	return NGX_OK;
}

static void
auge_stream_finalize_request(ngx_http_request_t* r, ngx_int_t rc)
{
	ngx_log_error(NGX_LOG_DEBUG, r->connection->log, 0, "auge_upstream_finalize_request");
}
