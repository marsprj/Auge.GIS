#include "ngx_http_auge_handler.h"

ngx_int_t 
ngx_postgres_handler(ngx_http_request_t *r)
{
	return NGX_OK;
}

void
ngx_postgres_wev_handler(ngx_http_request_t *r,ngx_http_upstream_t *u)
{
	ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "ngx_postgres_wev_handler");
}

void ngx_postgres_rev_handler(ngx_http_request_t *r,ngx_http_upstream_t *u)
{
	ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "ngx_postgres_rev_handler");
}

ngx_int_t
ngx_postgres_create_request(ngx_http_request_t *r)
{
	ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "ngx_postgres_create_request");
	r->upstream->request_bufs = NULL;
	return NGX_OK;
}

ngx_int_t
ngx_postgres_reinit_request(ngx_http_request_t *r)
{
	return NGX_OK;
}

void
ngx_postgres_abort_request(ngx_http_request_t *r)
{

}

void
ngx_postgres_finalize_request(ngx_http_request_t *r, ngx_int_t t)
{

}

ngx_int_t
ngx_postgres_process_header(ngx_http_request_t *r)
{
	return NGX_OK;
}

ngx_int_t
ngx_postgres_input_filter_init(void * p)
{
	return NGX_OK;
}

ngx_int_t 
ngx_postgres_input_filter(void *p, ssize_t s)
{
	return NGX_OK;
}
