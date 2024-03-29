# Webserver

This project is about writing own HTTP server in C++ 17 without using any external libraries.

The server supports the protocol HTTP/1.1. It handles GET, POST and DELETE methods.

## Compilation

```bash
# Compile server
make

# Recompile server
make re
```

## Configuring

Webserver takes config in nginx-like format.

### Directives

```
Syntax:     autoindex on | off;
Default:    autoindex off;
Context:    location
Example:    autoindex on;
```
Enables or disables the directory listing output.
###
```
Syntax:     cgi_pass extension path;
Default:    —
Context:    location
```
Sets the address of an CGI server.
###
```
Syntax:     client_max_body_size size;
Default:    client_max_body_size 1m;
Context:    server
```
Sets the maximum allowed size of the client request body.
###
```
Syntax:     error_page code path;
Default:    —
Context:    server
Example:    error_page 404 /404.html;
```
Defines the URI that will be shown for the specified errors.
###
```
Syntax:     index file ...;
Default:    index index.html;
Context:    location
```
Defines files that will be used as an index.
###
```
Syntax:     host address;
Default:    host *;
Context:    server
```
Sets the address for IP.
###
```
Syntax:     location uri { ... };
Default:    —
Context:    server
```
Sets configuration depending on a request URI.
###
```
Syntax:     limit_except method ...;
Default:    —
Context:    location
```
Defines a list of accepted HTTP methods for the route.
The method parameter can be one of the following: GET, POST, DELETE.
###
```
Syntax:     port port;
Default:    port 8080;
Context:    server
Example:    port 80;
```
Sets port for IP.
###
```
Syntax:     redirect code URL;
Default:    —
Context:    location
```
Redirects to the specified URL.
###
```
Syntax:     root path;
Default:    root html;
Context:    location
```
Sets the root directory for requests.
###
```
Syntax:     server { ... }
Default:    —
Context:    main
```
Sets configuration for a virtual server.
###
```
Syntax:     server_name name;
Default:    server_name "";
Context:    server
```
Sets names of a virtual server.
###
