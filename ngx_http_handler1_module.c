#include <nginx.h>
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

// module location config struct
typedef struct {
    ngx_flag_t enabled;
} ngx_http_handler1_loc_conf_t;

static ngx_int_t ngx_http_handler1_handler(ngx_http_request_t *r);
static ngx_int_t ngx_http_handler1_init(ngx_conf_t *cf);
static void* ngx_http_handler1_create_loc_conf(ngx_conf_t *cf);
static char* ngx_http_handler1_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);

// module directive
static ngx_command_t ngx_http_handler1_commands[] = {
    { ngx_string("handler1"),                          /* the directive string, no spaces */
      NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,                 /* flags that indicate where the directive is legal and how many arguments it takes. http://lxr.nginx.org/source/src/core/ngx_conf_file.h */
      ngx_conf_set_flag_slot,                          /* a pointer to a function for setting up part of the module's configuration. http://lxr.nginx.org/source/src/core/ngx_conf_file.h#L329 */
      NGX_HTTP_LOC_CONF_OFFSET,                        /* set if this value will get saved to the main, server or location configuration */
      offsetof(ngx_http_handler1_loc_conf_t, enabled), /* specifies which part of this configuration struct to write to */
      NULL },                                          /* just a pointer to other crap the module might need while it's reading the configuration */
      ngx_null_command
};

// module context
static ngx_http_module_t ngx_http_handler1_module_ctx = {
    NULL,                                /* preconfiguration */
    ngx_http_handler1_init,              /* postconfiguration */
    NULL,                                /* create main configuration */
    NULL,                                /* init main configuration */
    NULL,                                /* create server configuration */
    NULL,                                /* merge server configuration */
    ngx_http_handler1_create_loc_conf,   /* create location configuration */
    ngx_http_handler1_merge_loc_conf     /* merge location configuration */
};

// create location config
static void *
ngx_http_handler1_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_handler1_loc_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_handler1_loc_conf_t));
    if (conf == NULL) {
        return NGX_CONF_ERROR;
    }
    conf->enabled = NGX_CONF_UNSET;
    return conf;
}

// merge location config
static char *
ngx_http_handler1_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_handler1_loc_conf_t *prev = parent;
    ngx_http_handler1_loc_conf_t *conf = child;

    ngx_conf_merge_value(conf->enabled, prev->enabled, 0);

    return NGX_CONF_OK;
}

// the module definition
ngx_module_t ngx_http_handler1_module = {
    NGX_MODULE_V1,
    &ngx_http_handler1_module_ctx,     /* module context */
    ngx_http_handler1_commands,        /* module directives */
    NGX_HTTP_MODULE,                   /* module type */
    NULL,                              /* init master */
    NULL,                              /* init module */
    NULL,                              /* init process */
    NULL,                              /* init thread */
    NULL,                              /* exit thread */
    NULL,                              /* exit process */
    NULL,                              /* exit master */
    NGX_MODULE_V1_PADDING
};

// the handler
static ngx_int_t
ngx_http_handler1_handler(ngx_http_request_t *r)
{
    ngx_http_handler1_loc_conf_t* conf = ngx_http_get_module_loc_conf(r, ngx_http_handler1_module);

    if (!conf->enabled) {
        return NGX_DECLINED;
    }

    // generating a response
    u_char* some_bytes = (u_char*) "handler1 module here, everyone!";
    int some_bytes_length;
    for (some_bytes_length = 0; some_bytes[some_bytes_length] != '\0'; some_bytes_length++);

    // sending the header
    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = some_bytes_length;
    r->headers_out.content_type.len = sizeof("text/html") - 1;
    r->headers_out.content_type.data = (u_char *) "text/html";
    ngx_http_send_header(r);

    // sending the body
    ngx_buf_t    *b;
    ngx_chain_t   out;

    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
    if (b == NULL) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Failed to allocate response buffer.");
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    b->pos = some_bytes; /* first position in memory of the data */
    b->last = some_bytes + some_bytes_length; /* last position */

    b->memory = 1; /* content is in read-only memory */
    /* (i.e., filters should copy it rather than rewrite in place) */

    b->last_buf = 1; /* there will be no more buffers in the request */

    out.buf = b;
    out.next = NULL;

    return ngx_http_output_filter(r, &out);
}

// handler installation
static ngx_int_t
ngx_http_handler1_init(ngx_conf_t *cf)
{
    ngx_http_handler_pt        *h;
    ngx_http_core_main_conf_t  *cmcf;

    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

    h = ngx_array_push(&cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }

    *h = ngx_http_handler1_handler;

    return NGX_OK;
}
