#ifndef HANDLER_H_
#define HANDLER_H_

#include <http_server/server.h>

int solve_handler(HTTPRequest *request, HTTPResponse *response);
int preflight_handler(HTTPRequest *request, HTTPResponse *response);

#endif
