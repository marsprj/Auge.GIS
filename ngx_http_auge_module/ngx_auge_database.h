#include <libpq-fe.h>
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

/* connect database */
ngx_int_t auge_db_connect(PGconn** ppConnection, const char* server, const char* instance, const char* database, const char* user, const char* passwd);

/* close database */
ngx_int_t auge_db_close(PGconn* pConnection);

/* database query */
ngx_int_t auge_db_query(PGconn* pConnection, const char* table_name, ngx_http_request_t *r);
