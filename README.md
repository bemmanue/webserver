# Webserver

## Configuration file

### Directives

```
Syntax:     autoindex on | off;
Default:    autoindex off;
Context:    location
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
Syntax:     location uri;
Default:    —
Context:    server
```
Sets configuration depending on a request URI.
###
```
Syntax:     methods_allowed method ...;
Default:    —
Context:    location
```
Defines a list of accepted HTTP methods for the route.
The method parameter can be one of the following: GET, POST, DELETE.
###
```
Syntax:     port port;
Default:    port 8000;
Context:    server
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