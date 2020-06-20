#ifndef _RESPONDREQUEST_H
#define _RESPONDREQUEST_H

#include <unistd.h>
#include <stdio.h>

#include "ParseRequest.h"

int respondRequest(int pConnection, struct RequestInfo *pRequestInfo, char *pContentType);

#endif
