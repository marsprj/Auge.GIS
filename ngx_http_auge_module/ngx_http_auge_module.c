#include "ngx_http_auge_module.h"

typedef struct{
	ngx_str_t name;
}ngx_http_auge_loc_conf_t;

static ngx_command_t ngx_http_auge_commands[] = {
	{
		ngx_string("name"),
		NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS|NGX_CONF_TAKE1,
		ngx_http_auge_name,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_auge_loc_conf_t, name),
		NULL
	},

	ngx_null_command
};

static ngx_http_module_t ngx_http_auge_module_ctx = {
	NULL,					/* preconfiguration */
//	NULL,					/* postconfiguration */
	ngx_http_auge_init,			/* postconfiguration */
	NULL,					/* create main configuration */
	NULL,					/* init main configuration */
	NULL,					/* create server configuration */
	NULL,					/* merge server configuration */
	ngx_http_auge_create_loc_conf,		/* create location configuration */
	ngx_http_auge_merge_loc_conf		/* merge location configuration */
};

ngx_module_t ngx_http_auge_module = {
	NGX_MODULE_V1,
	&ngx_http_auge_module_ctx,
	ngx_http_auge_commands,
	NGX_HTTP_MODULE,
	NULL,
	NULL,
	ngx_http_auge_init_process,
	NULL,
	NULL,
	ngx_http_auge_exit_process,
	NULL,
	NGX_MODULE_V1_PADDING
};

static ngx_int_t
ngx_http_auge_handler(ngx_http_request_t *r)
{
	ngx_int_t rc;
	ngx_buf_t *b;
	ngx_chain_t out[2];

	u_char str[] = "Hello Auge";

	r->headers_out.content_type.len  = sizeof("text/plain") - 1;
	r->headers_out.content_type.data = (u_char*)"text/plain";

	b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));

	out[0].buf = b;
	out[0].next = NULL;

	b->pos = (u_char*)str;
	b->last = b->pos + sizeof(str);
	b->memory = 1;
	b->last_buf = 1;

	r->headers_out.status = NGX_HTTP_OK;
	r->headers_out.content_length_n = sizeof(str) - 1;
	rc = ngx_http_send_header(r);

	if(rc==NGX_ERROR || rc > NGX_OK)
	{
		return rc;
	}
	
	rc = ngx_http_output_filter(r, &out[0]);
	return rc;
}

static ngx_int_t 
ngx_http_auge_init(ngx_conf_t *cf)
{
	ngx_http_handler_pt        *h;
	ngx_http_core_main_conf_t  *cmcf;

	cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);
	h = ngx_array_push(&cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers);
	if (h == NULL) {
		return NGX_ERROR;
	}

	*h = ngx_http_auge_handler;
	return NGX_OK;
}

static void*
ngx_http_auge_create_loc_conf(ngx_conf_t *cf)
{
	ngx_http_auge_loc_conf_t *conf;
	
	conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_auge_loc_conf_t));
	if(conf == NULL)
	{
		return NGX_CONF_ERROR;
	}
	conf->name.len = 0;
	conf->name.data = NULL;

	return conf;
}

static char*
ngx_http_auge_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
	ngx_http_auge_loc_conf_t *prev = parent;
	ngx_http_auge_loc_conf_t *conf = child;

	ngx_conf_merge_str_value(conf->name, prev->name, "auge");
	
	return NGX_CONF_OK;
}

static char* 
ngx_http_auge_name(ngx_conf_t *cf, ngx_command_t *cmd, void* conf)
{
	ngx_http_core_loc_conf_t *clcf;
	clcf  = conf;
	char* rv = ngx_conf_set_str_slot(cf, cmd,conf);
	ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "[Auge Name]:%s",clcf->name.data);
	return rv;
}

static ngx_int_t 
ngx_http_auge_init_process(ngx_cycle_t *cycle)
{
	ngx_log_error(NGX_LOG_DEBUG, cycle->log, 0, "init auge process");
	return 0;
}

static void 
ngx_http_auge_exit_process(ngx_cycle_t *cycle)
{
	ngx_log_error(NGX_LOG_DEBUG, cycle->log, 0, "exit auge process");
}


