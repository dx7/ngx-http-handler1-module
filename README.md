nginx http handler1 module
==========================

An example handler module to nginx.

_This module is not distributed with the Nginx source. See [the installation instructions](#installation)._


Configuration
-------------

A location example:

      location /handler1 {
          handler1;
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
