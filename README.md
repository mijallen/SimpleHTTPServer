The following is a list of the proposed features of the HTTP server:

Core Goals:
 -handling requests with subprocesses (using fork)
 -handling requests with threads (using pthreads)
 -gathering file resources for responses (using dirent)
 -correctly handing common status codes (200, 400, 404, 500)
 -data structure for managing the filesystem

Optional Goals:
 -support for common gateway interface (possibly using pipe)
 -checking authorization (status codes 401, 403)
 -handling HTTP methods beyond GET and HEAD
 -configuration files based on YAML (also includes JSON)
