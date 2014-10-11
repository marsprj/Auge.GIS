#include "ngx_http_auge_module.h"
#include "ngx_http_auge_stream.h"

typedef struct 
{
	ngx_http_status_t	status;
}ngx_http_auge_ctx_t;

static ngx_command_t ngx_http_auge_commands[] = {
/*	{
		ngx_string("name"),
		NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS|NGX_CONF_TAKE1,
		ngx_http_auge_name,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_auge_loc_conf_t, name),
		NULL
	},*/

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
	ngx_http_auge_ctx_t *actx  = ngx_http_get_module_ctx(r, ngx_http_auge_module);
	if(actx==NULL)
	{
		actx = ngx_pcalloc(r->pool, sizeof(ngx_http_auge_ctx_t));
		if(actx == NULL)
		{
			return NGX_ERROR;
		}
		ngx_http_set_ctx(r, actx, ngx_http_auge_module);
	}

	if(ngx_http_upstream_create(r) != NGX_OK)
	{
		ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "ngx_uttp_upstream_create() failed");
		return NGX_ERROR;
	}

	ngx_http_auge_conf_t *acf = (ngx_http_auge_conf_t*)ngx_http_get_module_loc_conf(r, ngx_http_auge_module);
	ngx_http_upstream_t* u = r->upstream;

	u->conf = &acf->upstream;
	u->buffering = acf->upstream.buffering;

	u->resolved = (ngx_http_upstream_resolved_t*)ngx_pcalloc(r->pool, sizeof(ngx_http_upstream_resolved_t));
	if(u->resolved==NULL)
	{
		return NGX_ERROR;
	}

	static struct sockaddr_in backendSockAddr;
	struct hostent *pHost = gethostbyname((char*)"ourgis.digitalearth.cn");
	if(pHost==NULL)
	{
		return NGX_ERROR;
	}

	backendSockAddr.sin_family = AF_INET;
	backendSockAddr.sin_port = 80;
	char* pDmsIP = inet_ntoa(*(struct in_addr*)(pHost->h_addr_list[0]));
	backendSockAddr.sin_addr.s_addr = inet_addr(pDmsIP);
	actx->backendServer.data = (u_char*)pDmsIP;
	actx->backendServer.len = strlen(pDmsIP);

	u->resolved->sockaddr = (struct sockaddr*)&backendSockAddr;
	u->resolved->socklen = sizeof(struct sockaddr_in);

	u->create_request  = auge_upstream_create_request;
	u->process_header = auge_process_header;
	u->finalize_request = auge_stream_finalize_request;

	r->main->count++;
	ngx_http_upstream_init();

	return NGX_DONE;
}

/*tatic ngx_int_t
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
	ngx_http_auge_conf_t* acf;

	acf = (ngx_http_auge_conf_t*)ngx_pcalloc(cf->pool, sizeof(ngx_http_auge_conf_t));
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
	ngx_int_t ret = 0;
	ngx_http_auge_conf_t *prev = parent;
	ngx_http_auge_conf_t *conf = child;

	ngx_hash_init_t hash;
	hash.max_size = 100;
	hash.bucket_size = 1024;
	hash.name = "proxy _headers_hash";

	ret = ngx_http_upstream_hide_headers_hash(cf, 
						         &conf->upstream,
						         &prev->upstream,
						         ngx_http_proxy_hide_headers,
						         &hash);
	if(ret!=NGX_OK)
	{
		return NGX_CONF_ERROR;
	}
	
	return NGX_CONF_OK;
}

static ngx_int_t 
auge_process_status_line(ngx_http_request_t *r)
{
	size_t			len;
	ngx_int_t		rc;
	ngx_http_upstream_t	*u;

	ngx_http_auge_ctx_t *ctx = ngx_http_get_module_ctx(r, ngx_http_auge_module);
	if(ctx == NULL)
	{
		return NGX_ERROR;
	}

	u = r->upstream;

	rc = ngx_http_parse_status_line(r, &u->buffer, &ctx->status);
	if(rc==NGX_AGAIN){
		return rc;
	}

	if(rc!=NGX_ERROR){
		ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "upstream sent no valid HTTP/1.0 header");
		r->http_version = NGX_HTTP_9;
		r->state->status = NGX_HTTP_OK;

		return NGX_OK;
	}

	if(u->state){
		u->state->status = ctx->status.code;
	}

	u->headers_in.status_n = ctx->status.code;
	len = ctx->status.end - ctx.status.start;
	u->headers_in.status_line.len = len;

	u->headers_in.status_line.data = ngx_pnalloc(r->pool, len);
	if(u->headers_in.status_line.data == NULL){
		return NGX_ERROR;
	}

	ngx_memcpy(u->headers_in.status_line.data, ctx.status.start, len);
	return auge_upstream_process_header(r);
}

static ngx_int_t
auge_process_header(ngx_http_request_t *r)
{
	ngx_int_t 	rc;
	ngx_table_elt_t	*h;
	ngx_http_upstream_header_t *hh;
	ngx_http_upstream_main_conf_t *umcf;

	umcf = ngx_http_conf_get_module_main_conf(r, ngx_http_upstream_module);

	for(; ; )
	{
		rc = ngx_http_parse_status_line(r, &r->upstream->buffer, 1);
		if(rc==NGX_OK)
		{
			h = ngx_list_push(&r->upstream->headers_in.headers);
			if(h==NULL)
			{
				return NGX_ERROR;
			}
			h->hash = r->header->hash;

			h->key.len = r->header_name_end - r->header_name_start;
			h->value.len = r->header_end - r->headeer_start;

			h->key.data = ngx_pnalloc(r->pool, h->key.len+1 + h->value.len+1 + h->key.len);
			if(h->key.data == NULL)
			{
				return NGX_ERROR;
			}

			h->value.data = h->key.data + h->key.len + 1;
			h->lowcase_key = h->key.data + h->key.len + h->value.len + 1;

			ngx_memcpy(h->key.data, r->header_name_start, h->key.len);
			h->key.data[h->key.len] = '\0';
			ngx_memcpy(h->value.data, r->header_start, h->key.len);
			h->value.data[h->value.len] = '\0';
 
 			if (h->key.len == r->lowcase_index)
 			{
 				ngx_memcpy(h->lowcase_key, r->lowcase_header, h->key.len);
 			}
 			else
 			{
 				ngx_strlow(h->lowcase_key, r->key.data, h->key.len);
 			}

 			hh = ngx_hash_find(&umcf->headers_in_hash, h->hash, h->lowcase_key, h->key.len);

 			if(hh && hh->handler(r, h, hh->offset) != NGX_OK)
 			{
 				return NGX_ERROR;
 			}

 			continue;
		}

		if(rc==NGX_HTTP_PARSE_HEADER_DONE)
		{
			if(r->upstream->headers_in.server == NULL)
			{
				h = ngx_list_push(&r->upstream->headers_in.headers);
				if( h ==NULL)
				{
					return NGX_ERROR;
				}
				h->hash = ngx_hash(ngx_hash(ngx_hash(ngx_hash(ngx_hash('s','e'),'r'), 'v'), 'e'), 'r');

				ngx_str_set(&h->key, "server");
				ngx_str_null(&h->value);
				h->lowcase_key = (u_char*)"server";
			}

			if(r->upstream->headers_in.date==NULL)
			{
				h = ngx_list(&r->upstream->headers_in.headers);
				if(h==NULL)
				{
					return NGX_ERROR;
				}

				h->hash = ngx_hash(ngx_hash(ngx_hash('d', 'a'),'t'), 'a');

				ngx_str_set(&h->key, "Date");
				ngx_str_null(&h->value);
				h->lowcase_key = (u_char*)"date";
			}
		}

		return NGX_OK;
	}

	if(rc==NGX_AGAIN)
	{
		return NGX_AGAIN;
	}

	ngx_log_error(NGX_LOG_ERR, r->connection->log, -, "upstream send invalid header");

	return NGX_HTTP_UPSTREAM_INVALID_HEADER;
}

