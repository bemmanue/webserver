# Webserver

## Configuration file

### Directives

```
Syntax:     server { ... }
Default:    —
Context:    main
```
Sets configuration for a virtual server.

```
Syntax:     server_name name;
Default:    server_name "";
Context:    server
```
Sets names of a virtual server.

```
Syntax:     listen address[:port];
            listen port;
Default:    listen *:80 | *:8000;
Context:    server
```
Sets the address and port for IP.

```
Syntax:     error_page code ... [response] uri;
Default:    —
Context:    server
```
Defines the URI that will be shown for the specified errors.

```
Syntax:     client_max_body_size size;
Default:    client_max_body_size 1m;
Context:    server
```
Sets the maximum allowed size of the client request body.

```
Syntax:     allow_method method ...;
Default:    —
Context:    location
```
Defines a list of accepted HTTP methods for the route. 
The method parameter can be one of the following: GET, POST, DELETE.

```
Syntax:     redirect uri;
Default:    —
Context:    location
```
Redirects to the specified uri.

```
Syntax:     root path;
Default:    root html;
Context:    location
```
Sets the root directory for requests.

```
Syntax:     autoindex on | off;
Default:    autoindex off;
Context:    location
```
Enables or disables the directory listing output.

```
Syntax:     index file ...;
Default:    index index.html;
Context:    location
```
Defines files that will be used as an index.

```
Syntax:     cgi_pass address;
Default:    —
Context:    location
```
Sets the address of an CGI server.
