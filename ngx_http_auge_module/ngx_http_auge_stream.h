#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <string.h>

typedef struct{
	ngx_http_upstream_conf_t upstream;
}ngx_http_auge_conf_t;


typedef struct{
	ngx_uint_t	code;
	ngx_uint_t	count;
	u_char		*start;
	u_char		*end;
}ngx_http_auge_status_t;

static ngx_str_t  ngx_http_proxy_hide_headers[] =
{
    ngx_string("Date"),
    ngx_string("Server"),
    ngx_string("X-Pad"),
    ngx_string("X-Accel-Expires"),
    ngx_string("X-Accel-Redirect"),
    ngx_string("X-Accel-Limit-Rate"),
    ngx_string("X-Accel-Buffering"),
    ngx_string("X-Accel-Charset"),
    ngx_null_string
};

static ngx_int_t auge_upstream_create_request(ngx_http_request_t *r);
static void auge_stream_finalize_request(ngx_http_request_t* r, ngx_int_t rc);