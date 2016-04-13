#include <nginx.h>
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static ngx_int_t ngx_http_handler1_handler(ngx_http_request_t *r);
static char* ngx_http_handler1(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

// module directive
static ngx_command_t ngx_http_handler1_commands[] = {
    { ngx_string("handler1"),                     /* the directive string, no spaces */
      NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,          /* flags that indicate where the directive is legal and how many arguments it takes. http://lxr.nginx.org/source/src/core/ngx_conf_file.h */
      ngx_http_handler1,                          /* a pointer to a function for setting up part of the module's configuration. http://lxr.nginx.org/source/src/core/ngx_conf_file.h#L329 */
      NGX_HTTP_LOC_CONF_OFFSET,                   /* set if this value will get saved to the main, server or location configuration */
      0,                                          /* specifies which part of this configuration struct to write to */
      NULL },                                     /* just a pointer to other crap the module might need while it's reading the configuration */
      ngx_null_command
};

// module context
static ngx_http_module_t ngx_http_handler1_module_ctx = {
    NULL,                                /* preconfiguration */
    NULL,                                /* postconfiguration */
    NULL,                                /* create main configuration */
    NULL,                                /* init main configuration */
    NULL,                                /* create server configuration */
    NULL,                                /* merge server configuration */
    NULL,   /* create location configuration */
    NULL    /* merge location configuration */
};

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
    // generating a response
    u_char* some_bytes = (u_char*) "dx7 handler, everyone!";
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
static char *
ngx_http_handler1(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t  *clcf;

    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_handler1_handler;

    return NGX_CONF_OK;
}
