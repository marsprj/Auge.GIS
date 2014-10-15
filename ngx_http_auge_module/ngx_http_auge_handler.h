#ifndef NGX_HTTP_AUGE_HANDLER_H__
#define NGX_HTTP_AUGE_HANDLER_H__

#include <ngx_core.h>
#include <ngx_http.h>

ngx_int_t  ngx_postgres_handler(ngx_http_request_t *);
void       ngx_postgres_wev_handler(ngx_http_request_t *,
               ngx_http_upstream_t *);
void       ngx_postgres_rev_handler(ngx_http_request_t *,
               ngx_http_upstream_t *);
ngx_int_t  ngx_postgres_create_request(ngx_http_request_t *);
ngx_int_t  ngx_postgres_reinit_request(ngx_http_request_t *);
void       ngx_postgres_abort_request(ngx_http_request_t *);
void       ngx_postgres_finalize_request(ngx_http_request_t *, ngx_int_t);
ngx_int_t  ngx_postgres_process_header(ngx_http_request_t *);
ngx_int_t  ngx_postgres_input_filter_init(void *);
ngx_int_t  ngx_postgres_input_filter(void *, ssize_t);

#endif //NGX_HTTP_AUGE_HANDLER_H__
