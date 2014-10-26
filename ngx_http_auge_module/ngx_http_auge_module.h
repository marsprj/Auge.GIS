#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <string.h>

typedef struct {
//   ngx_postgres_upstream_peers_t      *peers;
//   ngx_uint_t                          current;
//    ngx_array_t                        *servers;
    ngx_pool_t                         *pool;
    /* keepalive */
    ngx_flag_t                          single;
    ngx_queue_t                         free;
    ngx_queue_t                         cache;
    ngx_uint_t                          active_conns;
    ngx_uint_t                          max_cached;
    ngx_uint_t                          reject;
} ngx_postgres_upstream_srv_conf_t;

static void* ngx_http_auge_create_loc_conf(ngx_conf_t *cf);
static char* ngx_http_auge_merge_loc_conf(ngx_conf_t *cf, void* parent, void *child);

static void* ngx_postgres_create_upstream_srv_conf(ngx_conf_t *cf);

static char* ngx_postgres_conf_server(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_int_t ngx_http_auge_init(ngx_conf_t *cf);

/* init auge process handler */
static ngx_int_t ngx_http_auge_init_process(ngx_cycle_t *cycle);
/* exit auge process handler */
static void ngx_http_auge_exit_process(ngx_cycle_t *cycle);