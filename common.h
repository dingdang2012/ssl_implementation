#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>

#define HTTP_PORT 80
#define HTTP_COMMAND_SIZE 100
#define BUF_SIZE 256

int parse_url(char *url, char **host, char **path);
int send_http_get(int fd, const char *path, const char *host);
int display_result(int fd);
