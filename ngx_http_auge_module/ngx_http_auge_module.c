#include "ngx_http_auge_module.h"

typedef struct{
	ngx_str_t name;
}ngx_http_auge_loc_conf_t;

typedef struct{
	ngx_str_t 	stock[6];
}ngx_http_auge_ctx_t;


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
	&ngx_http_auge_module_ctx,		/* module context */
	ngx_http_auge_commands,		/* module directives */
	NGX_HTTP_MODULE,			/* module type */
	NULL,					/* init master */
	NULL,					/* init module */
	ngx_http_auge_init_process,		/* init process */
	NULL,					/* init thread */
	NULL,					/* exit thread */
	ngx_http_auge_exit_process,		/* exit process */
	NULL,					/* exit master */
	NGX_MODULE_V1_PADDING
};

static ngx_int_t
ngx_http_auge_handler(ngx_http_request_t *r)
{
	ngx_int_t rc;
//	ngx_buf_t *b;
//	ngx_chain_t out[2];
//	ngx_http_auge_loc_conf_t* lcf;

	ngx_http_auge_ctx_t* actx = ngx_http_get_module_ctx(r, ngx_http_auge_module);
	if(actx == NULL)
	{
		actx = ngx_pcalloc(r->pool, sizeof(ngx_http_auge_ctx_t));
		if(actx == NULL)
		{
			return NGX_ERROR;
		}
		ngx_http_set_ctx(r, actx, ngx_http_auge_module);
	}

	ngx_http_post_subrequest_t* psr = ngx_pcalloc(r->pool, sizeof(ngx_http_post_subrequest_t));
	if(psr==NULL)
	{
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	psr->handler  = ngx_auge_subrequest_post_handler;
	psr->data = actx;

	ngx_str_t sub_prefix = ngx_string("/list=");
	ngx_str_t sub_location;
	sub_location.len = sub_prefix.len + r->args.len;
	sub_location.data = ngx_pcalloc(r->pool, sub_location.len);
	ngx_snprintf(sub_location.data, sub_location.len, "%V%V", &sub_prefix, &r->args);

	ngx_http_request_t* sr;
	rc =ngx_http_subrequest(r, &sub_location, NULL, &sr, psr, NGX_HTTP_SUBREQUEST_IN_MEMORY);
	if(rc != NGX_OK)
	{
		return NGX_ERROR;
	}

	return NGX_DONE;
}

/*static ngx_int_t
ngx_http_auge_handler(ngx_http_request_t *r)
{
	ngx_int_t rc;
	ngx_buf_t *b;
	ngx_chain_t out[2];
//	ngx_http_auge_loc_conf_t* lcf;
//	lcf = ngx_http_get_module_loc_conf(r, ngx_http_auge_module);

	ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0, "ngx_http_auge_handler is called!");

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
}*/

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

static ngx_int_t 
ngx_auge_subrequest_post_handler(ngx_http_request_t* r, void* data, ngx_int_t rc)
{
	ngx_http_request_t* pr = r->parent;

	ngx_http_auge_ctx_t* actx = ngx_http_get_module_ctx(pr, ngx_http_auge_module);
	pr->headers_out.status = r->headers_out.status;

	if(r->headers_out.status == NGX_HTTP_OK)
	{
		int flag = 0;
		ngx_buf_t* pRevBuf = &r->upstream->buffer;

		for(; pRevBuf->pos!=pRevBuf->last; pRevBuf->pos++)
		{
			if(*pRevBuf->pos==',' || *pRevBuf->pos == '\"')
			{
				if(flag >0)
				{
					actx->stock[flag-1].len = pRevBuf->pos-actx->stock[flag-1].data;
				}
				flag++;
				actx->stock[flag-1].data = pRevBuf->pos+1;
			}
			if(flag>6)
			{
				break;
			}
		}
	}
	pr->write_event_handler = ngx_auge_post_handler;
	return NGX_OK;
}

static void
ngx_auge_post_handler(ngx_http_request_t* r)
{
	if(r->headers_out.status != NGX_HTTP_OK)
	{
		ngx_http_finalize_request(r, r->headers_out.status);
		return;
	}

	ngx_http_auge_ctx_t* actx = ngx_http_get_module_ctx(r, ngx_http_auge_module);

	ngx_str_t output_format = ngx_string("stock[%V], Today current price: %V, volumn:%V");

	int bodylen = output_format.len + actx->stock[0].len + actx->stock[1].len + actx->stock[4].len - 6;
	r->headers_out.content_length_n = bodylen;

	ngx_buf_t* b = ngx_create_temp_buf(r->pool, bodylen);
	ngx_snprintf(b->pos, bodylen, (char*)output_format.data, &actx->stock[0], &actx->stock[1], &actx->stock[4]);
	b->last = b->pos + bodylen;
	b->last_buf = 1;

	ngx_chain_t out;
	out.buf = b;
	out.next = NULL;

	 static ngx_str_t type = ngx_string("text/plain; charset=GBK");
	 r->headers_out.content_type = type;
	 r->headers_out.status = NGX_HTTP_OK;

	 r->connection->buffered |= NGX_HTTP_WRITE_BUFFERED;
	 ngx_int_t ret  = ngx_http_send_header(r);
	 ret = ngx_http_output_filter(r, &out);

	 ngx_http_finalize_request(r, ret);

}
