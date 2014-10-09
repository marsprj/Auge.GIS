#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <string.h>

static char* ngx_http_auge_name(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static void* ngx_http_auge_create_loc_conf(ngx_conf_t *cf);
static char* ngx_http_auge_merge_loc_conf(ngx_conf_t *cf, void* parent, void *child);

static ngx_int_t ngx_http_auge_init(ngx_conf_t *cf);

/* init auge process handler */
static ngx_int_t ngx_http_auge_init_process(ngx_cycle_t *cycle);
/* exit auge process handler */
static void ngx_http_auge_exit_process(ngx_cycle_t *cycle);