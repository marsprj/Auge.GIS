#include "ngx_auge_database.h"

#include <libxml/tree.h>

/* connect database */
ngx_int_t auge_db_connect(PGconn** ppConnection, const char* server, const char* instance, const char* database, const char* user, const char* passwd)
{
	PGconn* pgConn = NULL;
	ConnStatusType status;

	char connstring[PATH_MAX] = {0};
	snprintf(connstring,  PATH_MAX, "hostaddr=%s port=%s dbname=%s user=%s password=%s", server, instance, database, user,  passwd);
	
	pgConn = PQconnectdb(connstring);
	status = PQstatus(pgConn);
	if(status != CONNECTION_OK)
	{
//		const char* err_msg = PQerrorMessage(pgConn);
		return -1;
	}
	*ppConnection = pgConn;
	return 0;
}

/* close database */
ngx_int_t auge_db_close(PGconn* pConnection)
{
	if(pConnection!=NULL)
	{
		PQfinish(pConnection);
	}
	return 0;
}

/* database query */
ngx_int_t auge_db_query(PGconn* pConnection, const char* table_name, ngx_http_request_t *r)
{
        ngx_int_t rc; 
        ngx_buf_t *b; 
        ngx_chain_t out[2];

//        u_char str[] = "Hello Auge xxx";
//        u_char str[] = "<root><name>xxx</name></root>";

	xmlDocPtr xmlDoc = xmlNewDoc((const xmlChar*)"1.0");
	xmlNodePtr xmlRoot = xmlNewNode(NULL, BAD_CAST "root");
	xmlDocSetRootElement(xmlDoc, xmlRoot);
	xmlNewChild(xmlRoot, NULL, BAD_CAST "name",BAD_CAST "content of node1");

	int bufSize = 0;
	xmlChar* xmlBuffer = NULL;
	xmlDocDumpFormatMemory(xmlDoc, &xmlBuffer, &bufSize, 1);


        r->headers_out.content_type.len  = sizeof("text/xml") - 1;
        r->headers_out.content_type.data = (u_char*)"text/xml";

        b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));

        out[0].buf = b;
        out[0].next = NULL;

        b->pos = (u_char*)xmlBuffer;
        b->last = b->pos + bufSize;
        b->memory = 1;
        b->last_buf = 1;

        r->headers_out.status = NGX_HTTP_OK;
        r->headers_out.content_length_n = bufSize - 1;
        rc = ngx_http_send_header(r);

        if(rc==NGX_ERROR || rc > NGX_OK)
        {   
                return rc; 
        }   
    
        rc = ngx_http_output_filter(r, &out[0]);

	xmlFree(xmlBuffer);
	xmlFreeDoc(xmlDoc);

	return rc;
}
