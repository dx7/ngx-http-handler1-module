nginx http handler1 module
==========================

An example handler module to nginx.

_This module is not distributed with the Nginx source. See [the installation instructions](#installation)._


Configuration
-------------

A location example:

      location /example {
          handler1 "on";
      }


Directives
----------

* **handler1** - enable handler1 on location


<a id="installation"></a>Installation instructions
--------------------------------------------------

[Download Nginx Stable](http://nginx.org/en/download.html) source and uncompress it (ex.: to ../nginx). You must then run ./configure with --add-module pointing to this project as usual. Something in the lines of:

    $ ./configure \
        --add-module=../ngx-http-handler1-module \
        --prefix=/usr/local/nginx
    $ make
    $ make install



Expected Result
---------------

    $ curl -i http://localhost:8080/example
    HTTP/1.1 200 OK
    Server: nginx/1.9.14
    Date: Wed, 13 Apr 2016 00:14:35 GMT
    Content-Type: text/html
    Content-Length: 31
    Connection: keep-alive

    handler1 module here, everyone!
