#include "ngx_http_auge_module.h"
#include "ngx_http_auge_handler.h"
#include "ngx_http_auge_upstream.h"

typedef struct{
	ngx_http_upstream_conf_t upstream;
}ngx_http_auge_loc_conf_t;

typedef struct{
	ngx_str_t 	stock[6];
}ngx_http_auge_ctx_t;

static ngx_command_t ngx_http_auge_commands[] = {
	 { ngx_string("postgres_server"),
	   NGX_HTTP_UPS_CONF|NGX_CONF_1MORE,
	   ngx_postgres_conf_server,
	   NGX_HTTP_SRV_CONF_OFFSET,
	   0,
	   NULL },
	ngx_null_command
};

static ngx_http_module_t ngx_http_auge_module_ctx = {
	NULL,					/* preconfiguration */
	ngx_http_auge_init,			/* postconfiguration */
	NULL,					/* create main configuration */
	NULL,					/* init main configuration */
	ngx_postgres_create_upstream_srv_conf,/* create server configuration */
	NULL,					/* merge server configuration */
	ngx_http_auge_create_loc_conf,	/* create location configuration */
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
/*
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
}*/

static ngx_int_t
ngx_http_auge_handler(ngx_http_request_t *r)
{
/*	ngx_int_t rc;
	ngx_buf_t *b;
	ngx_chain_t out[2];*/
	ngx_http_upstream_t *u = NULL;
	ngx_http_auge_ctx_t *actx = NULL;
	ngx_http_auge_loc_conf_t* alcf;
	
	actx = (ngx_http_auge_ctx_t*)ngx_http_get_module_ctx(r, ngx_http_auge_module);
	if(actx==NULL)
	{
		actx = ngx_pcalloc(r->pool, sizeof(ngx_http_auge_ctx_t));
		if(actx==NULL)
		{
			return NGX_HTTP_INTERNAL_SERVER_ERROR;
		}
		ngx_http_set_ctx(r, actx, ngx_http_auge_module);
	}	

	if(ngx_http_upstream_create(r)!=NGX_OK)
	{
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	alcf = (ngx_http_auge_loc_conf_t*)ngx_http_get_module_loc_conf(r, ngx_http_auge_module);

	u = r->upstream;

	u->schema.len = sizeof("postgres://") - 1;
	u->schema.data = (u_char*)"postgres://";

	u->output.tag = (ngx_buf_tag_t)&ngx_http_auge_module;
	u->conf = &(alcf->upstream);

	u->create_request = ngx_postgres_create_request;
	u->reinit_request = ngx_postgres_reinit_request;
	u->process_header = ngx_postgres_process_header;
	u->abort_request = ngx_postgres_abort_request;
	u->finalize_request = ngx_postgres_finalize_request;

	u->input_filter_init = ngx_postgres_input_filter_init;
	u->input_filter = ngx_postgres_input_filter;
	u->input_filter_ctx = NULL;

	ngx_http_upstream_init(r);

	u->write_event_handler = ngx_postgres_wev_handler;
	u->read_event_handler  = ngx_postgres_rev_handler;

	return NGX_DONE;

/*	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	return rc;*/
}

static ngx_int_t 
ngx_http_auge_init(ngx_conf_t *cf)
{
	ngx_http_handler_pt        *h;
	ngx_http_core_main_conf_t  *cmcf;

	ngx_http_upstream_srv_conf_t* uscf;

	cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);
	h = ngx_array_push(&cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers);
	if (h == NULL) {
		return NGX_ERROR;
	}

	*h = ngx_http_auge_handler;

	// init upstream
	uscf = ngx_http_conf_get_module_srv_conf(cf, ngx_http_upstream_module);
	uscf->peer.init_upstream = ngx_postgres_upstream_init;

	return NGX_OK;
}

static void*
ngx_http_auge_create_loc_conf(ngx_conf_t *cf)
{
	ngx_http_auge_loc_conf_t* acf;

	acf = (ngx_http_auge_loc_conf_t*)ngx_pcalloc(cf->pool, sizeof(ngx_http_auge_loc_conf_t));
	if(acf==NULL)
	{
		return NULL;
	}

	acf->upstream.connect_timeout = 60000;
	acf->upstream.send_timeout = 60000;
	acf->upstream.read_timeout = 60000;
	acf->upstream.store_access = 60000;

	acf->upstream.buffering = 0;
	acf->upstream.bufs.num = 9;
	acf->upstream.bufs.size = ngx_pagesize;
	acf->upstream.buffer_size = ngx_pagesize;
	acf->upstream.busy_buffers_size = 2*ngx_pagesize;
	acf->upstream.temp_file_write_size = 2*ngx_pagesize;
	acf->upstream.max_temp_file_size = 1024*1024*1024;
	acf->upstream.hide_headers = NGX_CONF_UNSET_PTR;
	acf->upstream.pass_headers = NGX_CONF_UNSET_PTR;

	return acf;
}

static char*
ngx_http_auge_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
	//ngx_http_auge_loc_conf_t *prev = parent;
	//ngx_http_auge_loc_conf_t *conf = child;

	//ngx_conf_merge_str_value(conf->name, prev->name, "auge");
	
	return NGX_CONF_OK;
}

void *
ngx_postgres_create_upstream_srv_conf(ngx_conf_t *cf)
{
    ngx_postgres_upstream_srv_conf_t  *conf;
//    ngx_pool_cleanup_t                *cln;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_postgres_upstream_srv_conf_t));
    if (conf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->peers = NULL
     *     conf->current = 0
     *     conf->servers = NULL
     *     conf->free = { NULL, NULL }
     *     conf->cache = { NULL, NULL }
     *     conf->active_conns = 0
     *     conf->reject = 0
     */

    conf->pool = cf->pool;

    /* enable keepalive (single) by default */
    conf->max_cached = 10;
    conf->single = 1;

    //cln = ngx_pool_cleanup_add(cf->pool, 0);
    //cln->handler = ngx_postgres_keepalive_cleanup;
    //cln->data = conf;

    return conf;
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

char *
ngx_postgres_conf_server(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
/*    ngx_str_t                         *value = cf->args->elts;
    ngx_postgres_upstream_srv_conf_t  *pgscf = conf;
    ngx_postgres_upstream_server_t    *pgs;
    ngx_http_upstream_srv_conf_t      *uscf;
    ngx_url_t                          u;
    ngx_uint_t                         i;
*/
//    ngx_http_upstream_srv_conf_t      *uscf;

//    uscf = ngx_http_conf_get_module_srv_conf(cf, ngx_http_upstream_module);


    return NGX_CONF_OK;
}