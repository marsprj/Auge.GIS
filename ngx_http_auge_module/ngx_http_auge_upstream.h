#ifndef _NGX_HTTP_AUGE_UPSTREAM_H_
#define _NGX_HTTP_AUGE_UPSTREAM_H_

#include <ngx_core.h>
#include <ngx_http.h>
#include <libpq-fe.h>

ngx_int_t ngx_postgres_upstream_init(ngx_conf_t *cf, ngx_http_upstream_srv_conf_t *uscf);
ngx_int_t ngx_postgres_upstream_init_peer(ngx_http_request_t *r, ngx_http_upstream_srv_conf_t *uscf);

ngx_int_t ngx_postgres_upstream_get_peer(ngx_peer_connection_t *pc, void *data);
void ngx_postgres_upstream_free_peer(ngx_peer_connection_t *pc, void *data, ngx_uint_t state);

#endif _NGX_HTTP_AUGE_UPSTREAM_H_
