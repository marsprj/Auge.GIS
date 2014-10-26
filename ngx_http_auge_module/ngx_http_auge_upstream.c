#include "ngx_http_auge_upstream.h"

ngx_int_t
ngx_postgres_upstream_init(ngx_conf_t *cf, ngx_http_upstream_srv_conf_t *uscf)
{
	uscf->peer.init = ngx_postgres_upstream_init_peer;

	return NGX_OK;
}

ngx_int_t
ngx_postgres_upstream_init_peer(ngx_http_request_t *r, ngx_http_upstream_srv_conf_t *uscf)
{
	ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "ngx_postgres_upstream_get_peer");

	ngx_http_upstream_t	*u;

	u = r->upstream;

	u->peer.get = ngx_postgres_upstream_get_peer;
	u->peer.free = ngx_postgres_upstream_free_peer;

	return NGX_OK;
}

ngx_int_t
ngx_postgres_upstream_get_peer(ngx_peer_connection_t *pc, void *data)
{
	ngx_log_error(NGX_LOG_ERR, pc->connection->log, 0, "ngx_postgres_upstream_get_peer");

	

	return NGX_OK;
}

void
ngx_postgres_upstream_free_peer(ngx_peer_connection_t *pc, void *data, ngx_uint_t state)
{
	ngx_log_error(NGX_LOG_ERR, pc->connection->log, 0, "ngx_postgres_upstream_free_peer");

	if(pc->connection)
	{
		//ngx_postgres_upstream_free_connection(pc->connection)
	}
}